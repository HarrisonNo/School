CREATE VIEW OutOfStateReservations AS
(
    SELECT C.custID, C.state AS cState, COUNT(*) AS outOfStateReservationCount
    FROM Customers C, ParkingLots PL, Reservations R
    WHERE C.custID=R.reserverID
    AND R.parkingLotID=PL.parkingLotID
    AND C.state!=PL.state
    AND (C.state IS NOT NULL AND PL.state IS NOT NULL)
    GROUP BY C.custID
    HAVING COUNT(*)>=3
)