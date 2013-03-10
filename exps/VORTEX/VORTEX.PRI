1		 	# all nanobots are the same
4
1:((G0 <= 0 ) && ((G1 != 0) || (G2 != 0))):3
2:G0==1 :1		# exactly 1 neighbor
3:G0==2 :2		# exactly 2 neighbors
4:1>0:0			# everything else is unimportant
