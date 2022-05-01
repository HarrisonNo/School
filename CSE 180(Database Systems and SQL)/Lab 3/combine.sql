BEGIN TRANSACTION ISOLATION LEVEL SERIALIZABLE;

UPDATE Reservations Re
SET reserverID=Mod.reserverID,parkingLotID=Mod.parkingLotID,reservationDate=Mod.reservationDate,startTime=Mod.startTime,endTime=Mod.endTime
FROM ModifyReservations Mod
WHERE Mod.reservationID=Re.reservationID;

INSERT INTO Reservations
SELECT Mod.reservationID,Mod.reserverID,Mod.parkingLotID,NULL,NULL,Mod.reservationDate,Mod.startTime,Mod.endTime,Mod.startTime,Mod.endTime
FROM ModifyReservations Mod
WHERE NOT EXISTS(SELECT *
                    FROM Reservations Re
                    WHERE Mod.reservationID=Re.reservationID);

COMMIT TRANSACTION;