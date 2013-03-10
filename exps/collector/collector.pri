2		 	# leader & followers different
2 			# 2 different imput symbols
1: G0 != 0 :1		# adjacent to wandering follower
2:  1  > 0 :0		# catch-all
3
1: G0 == 3  :2		# halt
2: G1 != 0  :1		# follow
3:  1 >  0  :0		# catch-all - keep wandering

