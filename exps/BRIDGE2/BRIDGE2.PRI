1		 	# all agents are the same
4			# only two inputs
1:G0==4 :1		# chasm to avoid
2:G0==3 :2              # target region
3:G0 != 4 && G0 != 3:0	# all other regions
4:1>0:0			# everything else is unimportant
