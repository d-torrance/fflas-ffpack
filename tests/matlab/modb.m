function Y= modb(X, p)

	 half_mod = (p-1)/2;
	 mhalf_mod = half_mod-p+1;
	 Y=mod(X,p);
	 m = find(Y < mhalf_mod);
	 n = find(Y > half_mod);
	 Y(m) = Y(m) + p;
	 Y(n) = Y(n) - p; 
end