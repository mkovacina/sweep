2		 	# leader & followers different
5			# 5 different imput symbols
1:G0==0 :0		# initial region
2:G0==1 :1		# edge region
3:G0==2 :2		# bridge region
4:G0==3 :3		# target region
5:G0==4 :4            	# chasm region
4
1:G1!=0 :1		# something to follow, so follow
2:G0==3 :3		# in target area
3:G0==4 :2		# in chasm
4:G1==0 :0		# nothing to do, so wander

