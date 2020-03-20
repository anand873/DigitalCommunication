function [dps_seq,lambda] = ProGen(N,NW,k)
    
    W = NW/N;
    
    d=((N-1-2*(0:N-1)').^2)*.25*cos(2*pi*W);
    ee=(1:N-1)'.*(N-1:-1:1)'/2;               
    
    B = spdiags([[ee;0] d [0;ee]],[-1 0 1],N,N);    %Create the Tridiagonal Matrix
    
    [V,~] = eigs(B,k);                              %Find the Eigenvectors
    
    Lv = k;
    
    dps_seq = zeros(N,k);
    
    for i = 1:k
        dps_seq(:,i) = V(:,i);
    end
    
    d=mean(dps_seq);
    
    for i=1:k
       if mod(i,2)==1
           if d(i)<0
               dps_seq(:,i)=-dps_seq(:,i); 
           end
       else
           if dps_seq(2,i)<0
               dps_seq(:,i)=-dps_seq(:,i); 
           end
       end
    end
    
    % Find the Eigenvalues
    
    s = [2*W; 4*W*sinc(2*W*(1:N-1)')];
    q = zeros(size(dps_seq));  
    for i=1:Lv:Lv
        blkind = i:min(i+Lv-1,Lv);
        q(:,blkind) = fftfilt(dps_seq(N:-1:1,blkind),dps_seq(:,blkind));
    end
    
    lambda = q'*flipud(s);
    lambda = min(lambda,1);
    lambda = max(lambda,0);
end

