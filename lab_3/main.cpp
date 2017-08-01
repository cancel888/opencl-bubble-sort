//
//  main.cpp
//  lab_3
//
//  Created by Nikita on 25.04.15.
//  Copyright (c) 2015 Nikita. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <OpenCL/OpenCL.h>
#include "kernel_exec.cl.h"

#define COUNT 4250

using namespace std;

int main(int argc, const char * argv[]) {
    ifstream in;
    ofstream out;
    
    in.open("in3", ios::in);
    out.open("out", ios::out);
    
    int n;
    int* m;
    int* m_out;
    
    in >> n;
    
    m = (int *) malloc(sizeof(int) * n);
    m_out = (int *) malloc(sizeof(int) * n);
    
    for (int i = 0; i < n; i++) {
        in >> m[i];
    }
    
    size_t offset = n <= COUNT ? 2 : 512;
    dispatch_queue_t queue = gcl_create_dispatch_queue(CL_DEVICE_TYPE_GPU, NULL);
    
    if (queue == NULL) {
        printf("GPU Problem..");
        
        return 0;
    }
    
    size_t cc;
    
    void* mem_in = gcl_malloc(sizeof(int) * n, m, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR);
    
    dispatch_sync(queue, ^{
        
        size_t wgs;
        
        gcl_get_kernel_block_workgroup_info(execute_kernel, CL_KERNEL_WORK_GROUP_SIZE, sizeof(wgs), &wgs, NULL);
        
        cl_ndrange range = { 1, {0, 0, 0}, {offset, 0, 0}, {0, 0, 0} };
        
        execute_kernel(&range, (cl_int *) mem_in, n, (int) offset);
        
        gcl_memcpy(m_out, mem_in, sizeof(cl_int) * n);
        
    });
    
    cout << cc << endl;
    
    for (int i = 0; i < n; i++) {
        out << m_out[i] << " ";
    }
    
    out << endl;
    
    return 0;
}































