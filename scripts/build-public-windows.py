# Littlefield Grid FTRLV publication tooling, 2026-11-22.
# SPDX-License-Identifier: LGPL-2.1-only
"""Fresh public-dependency Windows Release build; never packages or installs."""
from pathlib import Path
import argparse
import datetime
import json
import os
import re
import shutil
import subprocess
import sys


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build-dir', required=True)
    parser.add_argument('--cache-dir', required=True)
    parser.add_argument('--cmake')
    parser.add_argument('--autobuild')
    parser.add_argument('--git')
    parser.add_argument('--configure-only', action='store_true')
    parser.add_argument('--resume-public-attempt', action='store_true', help='Resume only this helper\'s recorded fresh public-download attempt')
    args = parser.parse_args()
    source = Path(__file__).resolve().parents[1]
    if not (source / 'PROVENANCE.md').is_file() or not (source / 'scripts/windows-public-variables').is_file():
        raise SystemExit('Use the curated public snapshot, not the development worktree.')
    build = Path(args.build_dir).resolve()
    cache = Path(args.cache_dir).resolve()
    previous = None
    if args.resume_public_attempt:
        previous = json.loads((build / 'public-build-result.json').read_text())
        if (previous.get('source') != str(source) or previous.get('build') != str(build)
                or previous.get('cache') != str(cache) or not previous.get('fresh_build')
                or not previous.get('fresh_installable_cache')
                or previous.get('status') not in ('configure_failed', 'compile_failed', 'build_succeeded')):
            raise SystemExit('Resume requires this source/build/cache pair and a recorded fresh public attempt (including incremental revalidation after publication-only edits).')
    for path in (build, cache):
        if path == source or source in path.parents or path in source.parents:
            raise SystemExit('Build/cache must be separate from the source directory.')
        if not previous and path.exists() and any(path.iterdir()):
            raise SystemExit('Fresh validation requires initially empty build and cache directories: ' + str(path))
    if build == cache or build in cache.parents or cache in build.parents:
        raise SystemExit('Build and cache must not overlap.')
    cmake = args.cmake or shutil.which('cmake')
    autobuild = args.autobuild or shutil.which('autobuild') or str(Path(sys.executable).with_name('autobuild.exe'))
    git = args.git or shutil.which('git')
    for tool in (cmake, autobuild, git):
        if not tool or not Path(tool).is_file():
            raise SystemExit('Install CMake, Git and autobuild 3.10.2 or supply explicit tool paths.')
    build.mkdir(parents=True, exist_ok=True)
    cache.mkdir(parents=True, exist_ok=True)
    env = os.environ.copy()
    removed = []
    for key in list(env):
        if re.search(r'TOKEN|SECRET|PASSWORD|CREDENTIAL|^AWS_|^AZURE_|^GOOGLE_APPLICATION|^SSH_AUTH_SOCK$|^GIT_ASKPASS$', key, re.I) or key in (
                'CMAKE_PREFIX_PATH', 'CMAKE_MODULE_PATH', 'LIBS_PREBUILT_DIR', 'AUTOBUILD_INSTALL_DIR',
                'AUTOBUILD_INSTALLABLE_CACHE', 'CL', '_CL_', 'CFLAGS', 'CXXFLAGS', 'LDFLAGS'):
            removed.append(key)
            env.pop(key, None)
    flags_file = source / 'scripts/windows-public-variables'
    flags = re.search(r'^LL_BUILD_WINDOWS_RELEASE="([^"]+)"$', flags_file.read_text(), re.M).group(1)
    empty_git = build / 'empty-gitconfig'
    empty_git.write_text('')
    env.update({
        'LL_BUILD': flags, 'revision': '61057', 'AUTOBUILD_BUILD_ID': '61057',
        'AUTOBUILD_ADDRSIZE': '64', 'AUTOBUILD_VARIABLES_FILE': str(flags_file),
        'AUTOBUILD_CONFIG_FILE': str(source / 'autobuild.xml'),
        'AUTOBUILD_INSTALLABLE_CACHE': str(cache), 'AUTOBUILD': str(autobuild),
        'PYTHONDONTWRITEBYTECODE': '1', 'GIT_TERMINAL_PROMPT': '0',
        'GIT_CONFIG_GLOBAL': str(empty_git), 'GIT_CONFIG_NOSYSTEM': '1',
        'GIT_OPTIONAL_LOCKS': '0', '_CL_': '/MP2',
    })
    env['PATH'] = ';'.join([str(Path(cmake).parent), str(Path(autobuild).parent), str(Path(git).parent), env.get('PATH', '')])
    switches = {
        'CMAKE_BUILD_TYPE:STRING': 'Release', 'ADDRESS_SIZE:STRING': '64',
        'ROOT_PROJECT_NAME:STRING': 'Kokua', 'VIEWER_CHANNEL:STRING': 'Littlefield Grid Kokua FTRLV Edition',
        'PYTHON_EXECUTABLE:FILEPATH': sys.executable, 'AUTOBUILD_EXECUTABLE:FILEPATH': str(autobuild),
        'GIT:FILEPATH': str(git), 'NVAPI:BOOL': 'ON', 'RLV_ALWAYS_ON:BOOL': 'ON',
        'USE_OPENAL:BOOL': 'ON', 'ENABLE_MEDIA_PLUGINS:BOOL': 'ON', 'UNATTENDED:BOOL': 'ON', 'NDOF:BOOL': 'ON',
    }
    for name in ('USE_VIVOX', 'USE_FMODSTUDIO', 'USE_KDU', 'USE_BUGSPLAT', 'USE_TRACY', 'USE_LTO',
                 'HAVOK', 'HAVOK_TPV', 'INSTALL_PROPRIETARY', 'PACKAGE', 'LL_TESTS', 'ENABLE_SIGNING'):
        switches[name + ':BOOL'] = 'OFF'
    configure = [str(cmake), '-S', str(source / 'indra'), '-B', str(build), '-G', 'Visual Studio 17 2022', '-A', 'x64']
    configure += ['-D' + name + '=' + value for name, value in switches.items()]
    compile_command = [str(cmake), '--build', str(build), '--config', 'Release', '--target', 'kokua-bin', '--', '/m:2', '/nologo', '/verbosity:minimal']
    record = {'started': datetime.datetime.now(datetime.timezone.utc).isoformat(), 'source': str(source),
              'build': str(build), 'cache': str(cache), 'fresh_build': True, 'fresh_installable_cache': True,
              'credentials_removed_names': sorted(removed), 'authentication': 'none supplied; interactive Git prompts disabled',
              'configure_command': configure, 'compile_command': compile_command, 'LL_BUILD': flags,
              'configure_exit_code': None, 'compile_exit_code': None, 'status': 'running'}
    result_file = build / 'public-build-result.json'
    if previous:
        attempt = 1
        while (build / f'public-build-attempt-{attempt}.json').exists():
            attempt += 1
        result_file.rename(build / f'public-build-attempt-{attempt}.json')
        for filename in ('configure.log', 'compile.log'):
            if (build / filename).exists():
                (build / filename).rename(build / f'{filename}.attempt-{attempt}')
        record['initial_fresh_started'] = previous.get('initial_fresh_started', previous['started'])
        record['resume_note'] = 'Reuses only the explicit cache/install tree from this recorded fresh anonymous-public-download attempt; no development cache is used.'
    result_file.write_text(json.dumps(record, indent=2))
    with (build / 'configure.log').open('wb') as log:
        result = subprocess.run(configure, cwd=source, env=env, stdout=log, stderr=subprocess.STDOUT)
    record['configure_exit_code'] = result.returncode
    if result.returncode:
        record['status'] = 'configure_failed'
    elif args.configure_only:
        record['status'] = 'configure_only_succeeded'
    else:
        with (build / 'compile.log').open('wb') as log:
            result = subprocess.run(compile_command, cwd=source, env=env, stdout=log, stderr=subprocess.STDOUT)
        record['compile_exit_code'] = result.returncode
        record['status'] = 'build_succeeded' if result.returncode == 0 else 'compile_failed'
    record['ended'] = datetime.datetime.now(datetime.timezone.utc).isoformat()
    result_file.write_text(json.dumps(record, indent=2))
    print(json.dumps(record, indent=2))
    return result.returncode


if __name__ == '__main__':
    sys.exit(main())
