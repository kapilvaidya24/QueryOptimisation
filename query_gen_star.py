import sys
import random
import math

# relation size prob.
# 10-100 15%
# 100-1,000 30%
# 1,000-10,000 25%
# 10,000-100,000 20%

# domain size prob.
# 2-10 5%
# 10-100 50%
# 100-500 35%
# 500-1,000 15%

def fsize():
	t=random.randint(1,101)	

	if(t<16):
		return (random.randint(0,90)+10)*100

	if(t<46):
		return (random.randint(0,900)+100)*100

	if(t<71):
		return (random.randint(0,9000)+1000)*100

	return (random.randint(0,90000)+10000)*100			

def dsize():
	t=random.randint(1,101)	

	if(t<6):
		return (random.randint(0,8)+2)*100

	if(t<56):
		return (random.randint(0,90)+10)*100

	if(t<91):
		return (random.randint(0,400)+100)*100

	return (random.randint(0,500)+500)*100			



n=1
s=1

if(len(sys.argv)<3):
	print("please give seed and size of star query as input")
else:
	s=int(sys.argv[1])
	n=int(sys.argv[2])
	
random.seed(s)

centre_size=fsize()

temp=[]
attr_num=n-1
for j in range(0,attr_num):
	temp.append(dsize())

centre_dom_size=temp

size_list=[]
sel_list=[]
dom_list=[]

for i in range(0,n-1):
	size_list.append(fsize())

	attr_num=random.randint(1,11)
	temp=[]

	for j in range(0,attr_num):
		temp.append(dsize())

	dom_list.append(temp)

for i in range(0,n-1):
	temp=0.00
	k=len(centre_dom_size)
	attr_ind=i

	temp=float(centre_dom_size[attr_ind])

	k=len(dom_list[i])
	attr_ind=random.randint(0,k-1)

	if(temp<float(dom_list[i][attr_ind])):
		temp=float(dom_list[i][attr_ind])

	sel_list.append(float(1/temp))	





t=float(centre_size)

alpha=1.0

for i in range(0,n-1):
	t=t*float(size_list[i])
	t=t*float(sel_list[i])

# print(str(t)+" is value of t")

if(t<1000000 or t>1000000000):
	t=t/10000000
	alpha=math.log(t,2)
	alpha=alpha/len(sel_list)
	alpha=math.pow(2,alpha)

# print(str(alpha)+" is alpha")

for i in range(len(sel_list)):
	# print(sel_list[i-1])
	sel_list[i]=sel_list[i]/alpha	
	# print(sel_list[i-1])




print(n)
print(n-1)

for i in range(1,n):
	s="1"
	for j in range(1,n):
		s=s+"0"
	print(s)
	s="0"	
	for j in range(1,i):
		s+="0"
	s+="1"
	for j in range(i,n-1):
		s+="0"
	print(s)		

	print(sel_list[i-1])

print(centre_size);
print(len(centre_dom_size));

for i in range(1,n):
	print(size_list[i-1])
	print(len(dom_list[i-1]))


# print(centre_size)

# for i in range(0,n-1):
# 	print(size_list[i])

# t=float(centre_size)


# for i in range(0,n-1):
# 	t=t*float(size_list[i])
# 	t=t*float(sel_list[i])

# print(t)








