function M=ff_rand_matrix(F, m, n)
    M=randi(2^20, m, n);
    M=modb(M,F);