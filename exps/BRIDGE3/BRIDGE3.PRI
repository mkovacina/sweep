1		 	# all agents are the same
5			# only two inputs
1:G0==4 :1		# chasm to avoid
2:G0==3 :2              # target region
3:G0==2 :3              # bridge
4:G0 == 0 || G0 == 1:0	# all other regions
5:1>0:0			# everything else is unimportant
