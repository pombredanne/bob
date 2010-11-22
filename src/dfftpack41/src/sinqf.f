C     _SUBROUTINE SINQF(N,X,WSAVE)
C
C     _SUBROUTINE SINQF COMPUTES THE FAST FOURIER TRANSFORM OF QUARTER
C     WAVE DATA. THAT IS , SINQF COMPUTES THE COEFFICIENTS IN A SINE
C     SERIES REPRESENTATION WITH ONLY ODD WAVE NUMBERS. THE TRANSFORM
C     IS DEFINED BELOW AT OUTPUT PARAMETER X.
C
C     SINQB IS THE UNNORMALIZED INVERSE OF SINQF SINCE A CALL OF SINQF
C     FOLLOWED BY A CALL OF SINQB WILL MULTIPLY THE INPUT SEQUENCE X
C     BY 4*N.
C
C     THE ARRAY WSAVE WHICH IS USED BY _SUBROUTINE SINQF MUST BE
C     INITIALIZED BY CALLING _SUBROUTINE SINQI(N,WSAVE).
C
C
C     INPUT PARAMETERS
C
C     N       THE LENGTH OF THE ARRAY X TO BE TRANSFORMED.  THE METHOD
C             IS MOST EFFICIENT WHEN N IS A PRODUCT OF SMALL PRIMES.
C
C     X       AN ARRAY WHICH CONTAINS THE SEQUENCE TO BE TRANSFORMED
C
C     WSAVE   A WORK ARRAY WHICH MUST BE DIMENSIONED AT LEAST 3*N+15.
C             IN THE _PROGRAM THAT CALLS SINQF. THE WSAVE ARRAY MUST
C             BE INITIALIZED BY CALLING _SUBROUTINE SINQI(N,WSAVE) AND
C             A DIFFERENT WSAVE ARRAY MUST BE USED FOR EACH DIFFERENT
C             VALUE OF N. THIS INITIALIZATION DOES NOT HAVE TO BE
C             REPEATED SO LONG AS N REMAINS UNCHANGED THUS SUBSEQUENT
C             TRANSFORMS CAN BE OBTAINED FASTER THAN THE FIRST.
C
C     OUTPUT PARAMETERS
C
C     X       FOR I=1,...,N
C
C                  X(I) = (-1)**(I-1)*X(N)
C
C                     + THE SUM FROM K=1 TO K=N-1 OF
C
C                     2*X(K)*SIN((2*I-1)*K*PI/(2*N))
C
C                  A CALL OF SINQF FOLLOWED BY A CALL OF
C                  SINQB WILL MULTIPLY THE SEQUENCE X BY 4*N.
C                  THEREFORE SINQB IS THE UNNORMALIZED INVERSE
C                  OF SINQF.
C
C     WSAVE   CONTAINS INITIALIZATION CALCULATIONS WHICH MUST NOT
C             BE DESTROYED BETWEEN CALLS OF SINQF OR SINQB.
C
      SUBROUTINE SINQF (N,X,WSAVE)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION       X(*)       ,WSAVE(*)
C
      IF (N .EQ. 1) RETURN
      NS2 = N/2
      DO 101 K=1,NS2
         KC = N-K
         XHOLD = X(K)
         X(K) = X(KC+1)
         X(KC+1) = XHOLD
  101 CONTINUE
      CALL COSQF (N,X,WSAVE)
      DO 102 K=2,N,2
         X(K) = -X(K)
  102 CONTINUE
      RETURN
      END
