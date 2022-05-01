SELECT DISTINCT PL.city AS theCity, PL.state AS theState
FROM ParkingLots PL, Reservations R
WHERE PL.parkingLotID NOT IN (SELECT R.parkingLotID 
                                FROM Reservations R)