# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(comb-oft) begin
child-oft: exit(3)
child-oft: exit(3)
child-oft: exit(3)
child-oft: exit(3)
child-oft: exit(3)
child-oft: exit(3)
(comb-oft) fie decriptors match
(comb-oft) end
comb-oft: exit(0)
EOF
pass;