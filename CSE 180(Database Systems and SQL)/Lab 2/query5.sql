SELECT C.custID, C.streetAddr
FROM Customers C, ParkingLots PL, Reservations R
WHERE (R.reserverID=C.custID)
    AND (PL.parkingLotID=R.parkingLotID)
    AND PL.city='New York'
    AND PL.state='NY'
    AND NOT EXISTS (SELECT R2.reservationID
                    FROM Customers C2, ParkingLots PL2, Reservations R2
                    WHERE (R2.reserverID=C2.custID)
                        AND (C.custID=C2.custID)
                        AND (PL2.parkingLotID=R2.parkingLotID)
                        AND PL2.city='New York'
                        AND PL2.state='NY'
                        AND R2.reservationID<>R.reservationID)