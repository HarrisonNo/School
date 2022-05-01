# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(comb-ewcorwc) begin
child-corwc: exit(1)
child-corwc: exit(2)
child-corwc: exit(3)
child-corwc: exit(4)
child-corwc: exit(5)
child-corwc: exit(6)
(comb-ewcorwc) c1 => 1
(comb-ewcorwc) c2 => 2
(comb-ewcorwc) c3 => 3
(comb-ewcorwc) c4 => 4
(comb-ewcorwc) c5 => 5
(comb-ewcorwc) c6 => 6
(comb-ewcorwc) end
comb-ewcorwc: exit(0)
EOF
pass;