all : client.c serverA.c serverB.c serverC.c aws.c
	gcc -o client client.c 
	gcc -o serverA serverA.c 
	gcc -o serverB serverB.c 
	gcc -o serverC serverC.c 
	gcc -o aws aws.c 
	
run_A: serverA
serverA:
	@(./serverA)
	
run_B: serverB
serverB:
	@(./serverB)
	
run_C: serverC
serverC:
	@(./serverC)

run_aws: aws
aws:
	@(./aws)
	
clean:
	$(RM) client serverA serverB serverC aws
	
.PHONY: serverA serverB serverC aws clean

