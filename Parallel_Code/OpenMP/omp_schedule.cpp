#include<stdio.h>
#include<omp.h>

int main() {
//  #pragma omp parallel for schedule(static) num_threads(4)//如果不加参数，那么size默认的就是迭代数除以线程总数，这里是分配给每个线程的任务都是一样的 
//  #pragma omp parallel for schedule(static, 5) num_threads(4)
//  #pragma omp parallel for schedule(dynamic) num_threads(4)//如果不加参数，那么size默认为1，这里是将size个任务分配给线程，做的快的可以先领取，因此大家做的任务数量不一定一样 
//  #pragma omp parallel for schedule(dynamic, 5) num_threads(4)
//  #pragma omp parallel for schedule(guided) num_threads(4)//于dynamic类似，不过其分配的任务数量是逐渐递减，但是不会小于size，size默认为1 
//  #pragma omp parallel for schedule(guided, 5) num_threads(4)
  for(int i = 0; i < 100; i++) {
  	printf("thread %d: i = %d\n", omp_get_thread_num(), i);
  } 
  return 0;
}
