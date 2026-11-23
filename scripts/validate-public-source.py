# Littlefield Grid FTRLV publication tooling, 2026-11-22.
# SPDX-License-Identifier: LGPL-2.1-only
"""Read-only portable static validation of the curated FTRLV source snapshot."""
from pathlib import Path
import ast
import hashlib
import json
import re
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[1]
checks = []


def check(value, description):
    assert value, description
    checks.append(description)


def read(path):
    return (ROOT / path).read_text(encoding='utf-8-sig')


commands = ET.parse(ROOT / 'indra/newview/app_settings/commands.xml').getroot()
check(sum(c.get('label_ref') == 'Command_LFG_Search_Label' for c in commands) == 1, 'one LFG Search command')
check(next(c for c in commands if c.get('name') == 'search').get('execute_parameters') == 'search_legacy', 'existing Search target preserved')
retired = {'legacy_search', 'marketplace', 'marketplacelistings', 'reporter', 'shop', 'buycurrency'}
check(not retired.intersection(c.get('name') for c in commands), 'retired toolbar commands absent')
check(not retired.intersection(c.get('name') for c in ET.parse(ROOT / 'indra/newview/app_settings/toolbars.xml').iter('command')), 'retired defaults absent')
for path in (ROOT / 'indra/newview/skins').rglob('menu*.xml'):
    for node in ET.parse(path).iter():
        assert not any(re.search(r'ReportAbuse|report_abuse|Report Abuse|BuyCurrency', value, re.I) for value in node.attrib.values()), path
        if path.name != 'menu_marketplace_view.xml':
            assert not any('marketplace' in value.lower() for value in node.attrib.values()), path
for path in (ROOT / 'indra/newview/skins').rglob('panel_status_bar.xml'):
    assert not any(node.get('name') in ('buyL', 'goShop') for node in ET.parse(path).iter()), path
check(True, 'removed menu/status-bar actions absent in all locales')
toolbar = read('indra/newview/lltoolbarview.cpp')
check('command == LLCommandId("legacy_search") ? LLCommandId("search")' in toolbar and 'hasCommand(migrated)' in toolbar, 'saved Search migration and deduplication retained')
status = read('indra/newview/llfloatergridstatus.cpp') + read('indra/newview/llstartup.cpp')
check('secondlifegrid' not in status and 'gAgent.getRegion() && !gDisconnected' in status, 'Grid Status has no Second Life status path')
toolmgr = read('indra/newview/lltoolmgr.cpp')
check(all(s in toolmgr for s in ('allowAgentBuild()', 'canEditSelection()', 'selectGetModify()', 'isPermanentEnforced()', 'mRRInterface.canEdit(object)')), 'Build and RLV permission checks retained')
chat = read('indra/newview/llchathistory.cpp')
check('getGrid() == LittlefieldEdition::GRID_KEY' in chat and '? LittlefieldEdition::GRID_LABEL' in chat and ': LLTrans::getString("SECOND_LIFE")' in chat, 'LFGrid chat prefix and other-grid fallback retained')
select = read('indra/newview/llselectmgr.cpp')
helper = re.search(r'bool LLSelectMgr::checkObjectPurchasePrice\([^)]*\)\s*\{(.*?)\n\}', select, re.S).group(1)
predicate = re.search(r'if \((.*?)\)\s*\{', helper, re.S).group(1)
expression = ' '.join(predicate.replace('LLGridManager::getInstance()->getGrid()', 'grid').replace('LittlefieldEdition::GRID_KEY', 'lfgrid').replace('sale_info.getSalePrice()', 'price').replace('&&', 'and').split())
tree = ast.parse(expression, mode='eval')
assert all(isinstance(n, (ast.Expression, ast.BoolOp, ast.And, ast.Compare, ast.Eq, ast.NotEq, ast.Name, ast.Load, ast.Constant)) for n in ast.walk(tree))
key = re.search(r'GRID_KEY\[\] = "([^"]+)"', read('indra/llcommon/lllittlefieldedition.h')).group(1)
for grid in (key, 'util.agni.lindenlab.com', 'util.aditi.lindenlab.com', 'other.example:8002', ''):
    for price in (0, 1, 10, 2147483647):
        assert eval(expression, {'__builtins__': {}}, {'grid': grid, 'lfgrid': key, 'price': price}) == (grid == key and price > 0)
check(True, '20 price/grid cases: LFGrid zero allowed and nonzero blocked; other grids unchanged')
check('return false;' in helper and 'return true;' in helper and 'setSalePrice' not in helper, 'price policy returns without rewriting price')
menu = read('indra/newview/llviewermenu.cpp')
buy = menu[menu.index('void handle_buy()'):]
check(buy.index('checkObjectPurchasePrice(sale_info)') < buy.index('gStatusBar->getBalance()'), 'price check precedes balance/funding path')
send = select[select.index('void LLSelectMgr::sendBuy('):]
check(send.index('checkObjectPurchasePrice(sale_info)') < send.index('sendListToRegions("ObjectBuy"'), 'price check precedes transaction dispatch')
for file in ('llfloaterbuy.cpp', 'llfloaterbuycontents.cpp'):
    text = read('indra/newview/' + file)
    assert 'checkObjectPurchasePrice(sale_info)' in text and 'checkObjectPurchasePrice(mSaleInfo)' in text
check(True, 'both purchase dialogs check opening and confirmation')
notice = ET.parse(ROOT / 'indra/newview/skins/default/xui/en/notifications.xml').getroot().find("notification[@name='LittlefieldObjectPurchaseUnavailable']")
expected = "Littlefield Grid \u2014 Purchase Unavailable\n\nThis object has a purchase price, but Littlefield Grid does not use an in-world currency. This purchase cannot be completed. Please contact the object's owner if you wish to obtain this item."
check(notice is not None and notice.text.strip() == expected and len(notice) == 1 and notice[0].get('name') == 'okbutton', 'exact purchase notice, acknowledgment only')
installer = read('indra/newview/installers/windows/littlefield_installer.nsi')
check('InstallDir "$PROGRAMFILES64\\LFGFTRLV"' in installer and 'Create Desktop Shortcut' in installer and 'Create Start Menu Shortcut' in installer, 'installation directory and shortcut choices retained')
check(not list((ROOT / '.github').glob('workflows/*')), 'no inherited Actions workflows enabled')
check(not (ROOT / '.git').exists(), 'no development Git metadata in source snapshot')
packages = json.loads(read('doc/dependencies-lock.json'))['packages']
check(len(packages) == 43 and len({p['name'] for p in packages}) == 43, 'all 43 dependency entries documented')
check('NOT READY' in read('doc/PUBLICATION_STATUS.md'), 'unresolved publication blockers remain explicit')
check('GNU LESSER GENERAL PUBLIC LICENSE' in read('LICENSE'), 'original LGPL license retained')

patterns = {
    'private-key': rb'-----BEGIN (?:RSA |EC |DSA |OPENSSH |ENCRYPTED )?PRIVATE KEY-----',
    'github-token': rb'\b(?:gh[pousr]_[A-Za-z0-9]{30,255}|github_pat_[A-Za-z0-9_]{30,255})\b',
    'aws-key-id': rb'\b(?:AKIA|ASIA)[A-Z0-9]{16}\b',
    'google-api-key': rb'\bAIza[0-9A-Za-z_-]{35}\b',
    'slack-token': rb'\bxox[baprs]-[0-9A-Za-z-]{20,}\b',
    'credential-url': rb'https?://[^\s/<>"\x27:]+:[^\s/<>"\x27@]+@',
}
hits = []
files = sorted(p for p in ROOT.rglob('*') if p.is_file())
text_count = 0
for path in files:
    relative = path.relative_to(ROOT).as_posix()
    assert not any(p in {'.git', '.aws', '.ssh', '__pycache__', 'release-artifacts', 'dependency-cache'} for p in path.relative_to(ROOT).parts), relative
    assert path.suffix.lower() not in ('.exe', '.dll', '.obj', '.lib', '.pdb', '.pfx', '.p12', '.key'), relative
    data = path.read_bytes()
    if b'\x00' in data[:4096]:
        continue
    text_count += 1
    for name, pattern in patterns.items():
        for match in re.finditer(pattern, data):
            hits.append({'file': relative, 'line': data.count(b'\n', 0, match.start()) + 1, 'category': name})
check(not hits, 'no high-confidence embedded secret/private-key patterns found')
manifest_path = ROOT / 'doc/SOURCE_MANIFEST.json'
if manifest_path.is_file():
    manifest = json.loads(manifest_path.read_text())
    actual = {p.relative_to(ROOT).as_posix() for p in files if p != manifest_path}
    check(actual == set(manifest['files']), 'staged file list matches curated manifest')
    for relative, entry in manifest['files'].items():
        assert hashlib.sha256((ROOT / relative).read_bytes()).hexdigest() == entry['sha256'], relative
    check(True, 'all curated manifest hashes match')
print(json.dumps({'static_validation': 'PASS', 'files': len(files), 'text_files_scanned': text_count,
                  'secret_hits': hits, 'checks': checks, 'publication_ready': False,
                  'note': 'A static PASS does not clear the explicit remaining binary corresponding-source, notice, runtime or immutable-installer artwork holds.'}, indent=2))
