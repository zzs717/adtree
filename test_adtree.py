import adtree
import numpy as np
from timeit import default_timer as timer

begin1 = timer()
ad = adtree.return_adtree(0,[0,1,0,0,1,2,0,0,0,0,1,1,2,0,2,1,1,0,0,2,0,1,2,1,2,1,0,0,1,3,0,1,2,2,2,0,1,0,1,3,0,2,0,0,0,1,2,2,1,1],10,5)

x = np.array([0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],dtype=np.int32)
begin2 = timer()
adtree.makecontab(ad,[1,2],x)
end = timer()
adtree.printTable(ad,x,[1,2])

print("the time to get contab(A,B)：")
print(end - begin2)


adtree.del_adtree(ad)
print("After deleting the tree:\n")
y = np.array([0,0,0],dtype=np.int32)
adtree.makecontab(ad,[1],y)
