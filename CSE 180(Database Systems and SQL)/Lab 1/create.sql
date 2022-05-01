DROP SCHEMA Lab1 CASCADE;
CREATE SCHEMA Lab1;

CREATE TABLE Customers(
    custID INT,
    PRIMARY KEY (custID),
    name VARCHAR(50),
    streetAddr VARCHAR(50),
    city VARCHAR(20),
    state CHAR(2),
    joinDate DATE,
    level CHAR(1),
    canReserve BOOLEAN
);

CREATE TABLE Vehicles(
    state CHAR(2),
    licensePlate CHAR(7),
    PRIMARY KEY (state, licensePlate),
    vehicleType CHAR(1),
    year INT,
    make VARCHAR(20),
    color CHAR(1)
);

CREATE TABLE ParkingLots(
    parkingLotID INT,
    PRIMARY KEY (parkingLotID),
    streetAddr VARCHAR(50),
    city VARCHAR(20),
    state CHAR(2),
    numSpots INT,
    costPerHour NUMERIC(5,2)
);

CREATE TABLE Reservations(
    reservationID INT,
    PRIMARY KEY (reservationID),
    reserverID INT,
    parkingLotID INT,
    state CHAR(2),
    licensePlate CHAR(7),
    reservationDate DATE,
    startTime TIME,
    endTime TIME,
    actualArrivalTime TIME,
    actualDepartureTime TIME
);

CREATE TABLE PaymentMethods(
    cardType CHAR(1),
    cardNum INT,
    PRIMARY KEY (cardType, cardNum),
    custID INT,
    expirationDate DATE,
    isValid BOOLEAN,
    FOREIGN KEY (custID) REFERENCES Customers(custID)
);

CREATE TABLE Payments(
    paymentID INT,
    PRIMARY KEY (paymentID),
    reservationID INT,
    cardType CHAR(1),
    cardNum INT,
    paymentDate DATE,
    amountPaid NUMERIC(6,2),
    FOREIGN KEY (reservationID) REFERENCES Reservations(reservationID),
    FOREIGN KEY(cardType, cardNum) REFERENCES PaymentMethods(cardType, cardNum)
);