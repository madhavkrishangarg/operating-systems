all:
	mkfifo ques2
	gcc fifo_receiver.c -o fifo_p2
	gcc fifo_sender.c -o fifo_p1
	gcc sm_receiver.c -o sm_p2
	gcc sm_sender.c -o sm_p1
	gcc ds_receiver.c -o ds_p2
	gcc ds_sender.c -o ds_p1