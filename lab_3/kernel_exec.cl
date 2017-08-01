
kernel void execute(global int* in, int size, int offset) {
    
    //int i = get_work_dim() * get_group_id(0) + get_global_id(0) * 2;
    int i = get_global_id(0) * 2;
    
    int first;
    int second;
    int third;
    
    if (i < size) {
        for (int j = 0; j < size; j++) {
            
            for (int k = i; k < size; k += offset) {
                
                if (k + 1 < size) {
                    first = in[k];
                    second = in[k + 1];
                    
                    if (first > second) {
                        int tmp = first;
                        in[k] = second;
                        in[k + 1] = tmp;
                    }
                }
            }
            
            barrier(CLK_GLOBAL_MEM_FENCE | CLK_LOCAL_MEM_FENCE);
            
            for (int k = i; k < size; k += offset) {
                if (k + 2 < size) {
                    second = in[k + 1];
                    third = in[k + 2];
                    
                    if (second > third) {
                        int tmp = second;
                        in[k + 1] = third;
                        in[k + 2] = tmp;
                    }
                }
            }
            
            barrier(CLK_GLOBAL_MEM_FENCE | CLK_LOCAL_MEM_FENCE);
            
        }

    }
    
    //barrier(CLK_GLOBAL_MEM_FENCE | CLK_LOCAL_MEM_FENCE);
}