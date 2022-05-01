
-- CSE 180 Winter 2022 Lab2 create.sql which you're asked to modify

-- Note that Vehicles has an attribute, OwnerID that didn't appear in Lab1.
-- Also there's a Foreign Key constraint on OwnerID, requiring that every OwnerID 
--       be a custom in Customers.

-- Following two lines are not needed in your solution.
DROP SCHEMA Lab2 CASCADE;
CREATE SCHEMA Lab2;

-- The comments that appear below are not needed in your solution.

-- INT is equivalent to INTEGER.
-- DECIMAL is equivalent to NUMERIC.
-- CHAR is equivalent to CHARACTER.
-- Specific spacing doesn't matter.

-- Customers(custID, name, streetAddr, city, state, joinDate, level, canReserve)
CREATE TABLE Customers(
    custID INT PRIMARY KEY,
    name VARCHAR(50),
    streetAddr VARCHAR(50),
    city VARCHAR(20),
    state CHAR(2),
    joinDate DATE,
    level CHAR,
    canReserve BOOLEAN,
    UNIQUE (name)
);

-- Primary Key custID could appear as a schema element, instead of next to attribute.


--Vehicles(state, licensePlate, vehicleType, year, make, color)
CREATE TABLE Vehicles(
    state CHAR(2),
    licensePlate CHAR(7),
    vehicleType CHAR NOT NULL,
    year INT,
    make VARCHAR(20),
    color CHAR,
    ownerID INT,
    PRIMARY KEY (state, licensePlate),
    FOREIGN KEY (ownerID) REFERENCES Customers(custID)
);

-- Foreign Key specification for ownerID could appear next to attribute, 
--     instead of as a schema element.


-- ParkingLots(parkingLotID, streetAddr, city, state, numSpots, costPerHour)
CREATE TABLE ParkingLots(
    parkingLotID INT PRIMARY KEY,
    streetAddr VARCHAR(50),
    city VARCHAR(20),
    state CHAR(2),
    numSpots INT,
    costPerHour NUMERIC(5,2) NOT NULL,
    UNIQUE (streetAddr,city,state)
);

-- Primary Key parkingLotID could appear as a schema element, instead of next to attribute.


-- Reservations(reservationID, reserverID, parkingLotID, state, licensePlate, reservationDate,
--               startTime, endTime, actualArrivalTime, actualDepartureTime)
CREATE TABLE Reservations(
    reservationID INT PRIMARY KEY,
    reserverID INT,
    parkingLotID INT,
    state CHAR(2),
    licensePlate CHAR(7),
    reservationDate DATE,
    startTime TIME,
    endTime TIME,
    actualArrivalTime TIME,
    actualDepartureTime TIME,
    UNIQUE (reserverID,reservationDate),
    FOREIGN KEY (reserverID) REFERENCES Customers(custID),
    FOREIGN KEY (parkingLotID) REFERENCES ParkingLots(parkingLotID),
    FOREIGN KEY (state, licensePlate) REFERENCES Vehicles(state, licensePlate)
);

-- Primary Key reservationID could appear as a schema element, instead of next to attribute.
-- Foreign Key specifications don't have to mention attribute names after REFERENCES when the
--   attribute names are identical in both the referencing and the referenced relations.
-- Foreign Keys involving a single attribute may be specified next to that attribute, rather than
--   as a schema element, as in:   reserverID INT REFERENCES Customers(custID)


-- PaymentMethods(cardType, cardNum, custID, expirationDate, isValid)
CREATE TABLE PaymentMethods(
    cardType CHAR,
    cardNum INT,
    custID INT,
    expirationDate DATE NOT NULL,
    isValid BOOLEAN,
    PRIMARY KEY (cardType, cardNum),
    FOREIGN KEY (custID) REFERENCES Customers(custID)
);

-- FOREIGN KEY (custID) REFERENCES Customers    is also okay.
-- Foreign Keys involving a single attribute may be specified next to that attribute, rather than
--   as a schema element, as in:   reserverID INT REFERENCES Customers(custID)
--   or:    reserverID INT REFERENCES Customers


-- Payments(paymentID, reservationID, cardType, cardNum, paymentDate, amountPaid)
CREATE TABLE Payments(
    paymentID INT PRIMARY KEY,
    reservationID INT,
    cardType CHAR,
    cardNum INT,
    paymentDate DATE,
    amountPaid NUMERIC(6,2),
    FOREIGN KEY (reservationID) REFERENCES Reservations(reservationID),
    FOREIGN KEY (cardType, cardNum) REFERENCES PaymentMethods(cardType, cardNum)
);

-- Primary Key paymentID could appear as a schema element, instead of next to attribute.
-- Foreign Key specifications don't have to mention attribute names after REFERENCES when the
--   attribute names are identical in both the referencing and the referenced relations.
-- Foreign Keys involving a single attribute may be specified next to that attribute, rather than
--   as a schema element, as in:   reservationID INT REFERENCES Reservation(resevationID)
--   or:    rreservationID INT REFERENCES Reservation
