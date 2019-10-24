( m" Тарбазанов " string-hash 3 % .
2 )

: parity 2 % if ." no" else ." yes" then ;

: prime
dup 0 < if ." negative argument " else
dup 2 < if drop 0 else
dup 4 < if drop 1 else
dup
dup 2 / 1 + 2
for r@ % 0 = if drop 0 r> drop r@ >r
else dup then
endfor
0 = if 0 else drop 1 then
then
then
then ;

: allot-prime
    prime
    cell% allot dup
    rot swap
    !
;


: concat ( pointer1 ponter2 -- pointer3 )
    ( pointer1 pointer2 )
    dup count
    ( pointer1 pointer2 length2 )
    rot
    ( pointer2 length2 pointer1 )
    dup count
    ( pointer2 length2 pointer1 length1 )
    rot swap
    ( pointer2 pointer1 length2 length1 )
    dup rot
    ( pointer2 pointer1 length1 length1 length2 )
    + heap-alloc
    ( pointer2 pointer1 length1 pointer3 )
    rot dup
    ( pointer2 length1 pointer3 pointer1 pointer1 )
    rot dup
    ( pointer2 length1 pointer1 pointer1 pointer3 pointer3 )
    rot string-copy
    ( pointer2 length1 pointer1 pointer3 )
    swap heap-free
    ( pointer2 length1 pointer3 )
    dup rot
    ( pointer2 pointer3 pointrr3 length1 )
    + rot
    ( pointer3 pointer3-2 pointer2 )
    dup rot swap
    ( pointer3 pointer2 pointer3-2 pointer2 )
    string-copy heap-free
    ( pointer3 )
;

( n -- next_prime )
: next-prime
repeat
( x )
1 +
( x++ )
dup prime
( x++ is_x++_prime )
until ;

( this word devides a by b^n where n is max avaliable )
: test repeat 2dup / rot drop swap 2dup % until ;

: radical
dup 2 < if ." cannot solve radical for 1 " else 
( n )
1
( n 1 )
swap
( 1 n )
1 swap
( 1 1 n )
repeat
swap
( radical new_n prime )
( 1 n 1 )
next-prime
( radical new_n next_prime )
2dup
( radical new_n next_prime new_n next_prime )
% not if
( 1 n 2 )
test
( 1 new_n 2 )
rot 2dup
( new_n 2 1 2 1 )
*
( new_n 2 1 radical )
swap drop
( new_n 2 radical )
rot rot
( radical new_n 2 )
swap
( radical 2 new_n )
dup 1
( radical 2 new_n new_n 1)
=  
else swap 0 then  ( 0 not to end repeat until ) 
until rot then ;





