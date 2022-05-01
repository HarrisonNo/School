SELECT DISTINCT P.paymentID, PM.custID
FROM Payments P, PaymentMethods PM, Customers C, ParkingLots PL, Reservations R
WHERE P.amountPaid<123.99
    AND PM.isValid IS NULL
    AND (PM.cardNum=P.cardNum AND PM.cardType=P.cardType)
    AND (C.custID=PM.custID)
    AND C.canReserve IS FALSE
    AND (C.custID=R.reserverID)
    AND (R.parkingLotID=PL.parkingLotID)
    AND PL.state='NY'
    AND PL.city='New York'