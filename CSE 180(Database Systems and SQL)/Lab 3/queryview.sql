SELECT C.custID, C.state AS cState, V.state AS vState
FROM Customers C, Vehicles V, OutOfStateReservations OOS
WHERE C.custID=OOS.custID
AND C.custID=V.ownerID
AND C.state!=V.state;
/*RESULTS OF INITIAL QUERY
custid | cstate | vstate
-------------------------
   104 | ON     | NJ
   20  | MN     | NJ
   20  | MN     | ON
*/
DELETE FROM Reservations
WHERE reservationID=21923;
DELETE FROM Reservations
WHERE reservationID=29223;
/*RESULTS OF QUERY WITH DELETES
custid | cstate | vstate
-------------------------
   104 | ON     | NJ
*/