1				# all nanobots are the same
9
1:abs(G0) == 1 && abs(G1) == 1 :0	# avoid corners
2:G0 ==  1	:1		# info  right
3:G0 == -1	:2		# info left
4:G1 == -1	:3		# info up
5:G1 ==  1	:4		# info down
6:abs(G0) == 2 && abs(G1) == 2 :0
7:abs(G0) == 2  :5
8:abs(G1) == 2  :6
9:1>0		:0	# everything else doesn't matter


