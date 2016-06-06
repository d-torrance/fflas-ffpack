function M=ff_init_Matrix(F, m, n)
    M=modb(randi(2^20, m, n), F);
    