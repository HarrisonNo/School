INSERT INTO PaymentMethods(cardType,cardNum,custID)
VALUES('V',12345,6942069);

INSERT INTO Payments(paymentID,reservationID)
VALUES(12345,6942069);

INSERT INTO Payments(paymentID,cardType,cardNum)
VALUES(5325,'V',54398);



UPDATE ParkingLots
SET costPerHour=5;

UPDATE ParkingLots
SET costPerHour=-5;


UPDATE Reservations
SET startTime='16:00:00',endTime='20:00:00',actualArrivalTime='17:33:30',actualDepartureTime='19:58:44';

UPDATE Reservations
SET startTime='20:00:00',endTime='16:00:00',actualArrivalTime='19:58:44',actualDepartureTime='17:33:30';


UPDATE Payments
SET cardType=NULL,cardNum=NULL;

UPDATE Payments
SET cardType=NULL,cardNum=5425;