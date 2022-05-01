ALTER TABLE ParkingLots
ADD CONSTRAINT positiveCostPerHour CHECK (costPerHour>0);

ALTER TABLE Reservations
ADD CONSTRAINT timeGoesForward CHECK (startTime<=endTime AND actualArrivalTime<=actualDepartureTime);

ALTER TABLE Payments
ADD CONSTRAINT validCard CHECK (cardType IS NOT NULL OR (cardType IS NULL AND cardNum IS NULL));