function [] = Main()
    C = [0.5 1 2 4];
    
    for c=1:4
       len=512;
       
       % Generating PSWFs %
       
       [PSWF,L] = ProGen(len,C(c),5);
       figure;
       plot(PSWF);
       title(['Slepian Sequences, N = 512, c = ',num2str(C(c))]);
       axis([1 512 -0.15 0.15]);
       legend('1st','2nd','3rd','4th','5th');
       fprintf('\nLambda values for c = %.1f\n',C(c));
       for i=1:5
           fprintf('\tn = %d\tLambda = %.6f\n',i,L(i));
       end
       
       % Verify Properties %
       
       fprintf('\nMutual Dot Product (Infinite Support) Table for c = %.1f\n',C(c));
       for i=1:5
           for j=1:5
                A=PSWF(:,i);
                B=PSWF(:,j);
                x=dot(A,B);
                fprintf('%.0f\t',abs(x));
           end
           fprintf('\n');
       end
       fprintf('\n');
       
       fprintf('\nMutual Dot Product (Time Limited) Table for c = %.1f\n',C(c));
       for i=1:5
           for j=1:5
                A=PSWF(57:456,i);
                B=PSWF(57:456,j);
                x=dot(A,B);
                fprintf('%.3f\t',abs(x));
           end
           fprintf('\n');
       end
       fprintf('\n');
       
        for i = 1:5
          A = PSWF(:,i);
          F_out = Limit(A,400,C(c),i);
          
          Err=0;
          sum=0;
          for j =1: length(F_out)
              sum=sum+abs(A(j));
              Err=Err+(F_out(j)-A(j)*L(i))^2;
          end
           
          Err=sqrt(Err)/sum;
          fprintf('Relative Error in BD(F) and \x03bb(F) for c = %f and Order = %d is %.3f\n',C(c),i,abs(Err));         
        end    
       
       % Expanding a Given Function %
       
       x = [-2.56:0.01:2.56-0.01];
       y = exp(-x.^2);              % Arbitrary L2 Function
        
       % Finite Support %
       
       Seq = zeros(5,512);
       for i = 1:5
            for j =107:406
                Seq(i,j) = PSWF(j,i);
            end
       end
       Coeff = Expnd(y,Seq.');
       fprintf('\nCoefficients of Expansion (Finite Support) (c = %d):\n',C(c));
       disp(Coeff);
       Sig=zeros(size(Seq(i,:)));
       for i=1:5
           Sig = Sig + Coeff(i)*Seq(i,:);
       end
       figure
       plot(x,y)
       hold on;
       plot(x,Sig);
       hold off;
       title(['Comparision of Signal Using Bases (Finite Support) with Original, c = ',num2str(C(c))]);
       axis([-2.56 2.56 -1 1.5 ]);
       legend('Original','Using Bases');
       
       %Energy Difference:
       Ener_Orig = dot(y,y);
       Ener_New = dot(Sig,Sig);
       
       fprintf('\nEnergy Ratio: %.3f\n',Ener_New/Ener_Orig) ;
       
       %Infiinite Support
       
       Coeff = Expnd(y,PSWF);
       fprintf('\nCoefficients of Expansion (Infinite Support)(c = %d):\n',C(c));
       disp(Coeff);
       Sig=zeros(size(PSWF(:,i)));
       for i=1:5
           Sig = Sig + Coeff(i)*PSWF(:,i);
       end
       figure
       plot(x,y)
       hold on;
       plot(x,Sig);
       hold off;
       title(['Comparision of Signal Using Bases (Infinite Support) with Original, c = ',num2str(C(c))]);
       axis([-2.56 2.56 -1 1.5 ]);
       legend('Original','Made using Bases');
       
       %Energy Difference:
       Ener_Orig = dot(y,y);
       Ener_New = dot(Sig,Sig);
       fprintf('\nEnergy Ratio: %.3f\n',Ener_New/Ener_Orig) ;
       
    end
end

