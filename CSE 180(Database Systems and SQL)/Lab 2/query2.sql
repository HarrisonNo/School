SELECT C.custID, C.name, R.reservationID, PL.streetAddr
FROM Customers C, Reservations R, ParkingLots PL
WHERE R.reservationDate='2021-07-28' 
    AND R.actualArrivalTime<R.startTime 
    AND R.actualDepartureTime>R.endTime 
    AND R.reserverID=C.custID 
    AND R.parkingLotID=PL.parkingLotID