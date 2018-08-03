import re
import sys

if len(sys.argv) < 2:
    print('Usage: '+sys.argv[0]+' <function name>')
    sys.exit()

name = sys.argv[1]

patt = re.compile('define[^@]+@([^\(]+'+name+'[^\(]+)\(')
for line in sys.stdin.readlines():
    if line.find(name) != -1:
        m_obj = patt.search(line)
        if m_obj:
            print(m_obj.group(1))
            break

