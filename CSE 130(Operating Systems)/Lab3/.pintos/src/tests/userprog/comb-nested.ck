# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(comb-nested) begin
(child-simple) run
child-simple: exit(1923)
(child-simple) run
child-simple: exit(1923)
(child-simple) run
child-simple: exit(1923)
(child-simple) run
child-simple: exit(1923)
(child-simple) run
child-simple: exit(1923)
child-nested: exit(0)
(child-simple) run
child-simple: exit(1923)
child-nested: exit(0)
(child-simple) run
child-simple: exit(1923)
(child-simple) run
child-simple: exit(1923)
(child-simple) run
child-simple: exit(1923)
child-nested: exit(0)
(comb-nested) end
comb-nested: exit(0)
EOF
pass;