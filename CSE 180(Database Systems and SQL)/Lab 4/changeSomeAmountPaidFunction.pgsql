CREATE OR REPLACE FUNCTION
changeSomeAmountPaidFunction(theCustID INTEGER, maxReduction INTEGER)
RETURNS INTEGER AS $$

    DECLARE 
    amountReduced INTEGER;/*Amount actually reduced*/

    DECLARE theReservation record;
    DECLARE reductionCursor CURSOR FOR
        SELECT P.reservationID, P.amountPaid
        FROM PaymentMethods PM, Payments P
        WHERE PM.custID=theCustID AND PM.cardType=P.cardType AND PM.cardNum=P.cardNum
        GROUP BY P.reservationID, P.amountPaid, P.paymentDate, P.amountPaid
        ORDER BY P.paymentDate DESC, P.amountPaid DESC;



    BEGIN
    IF maxReduction<0 THEN
        RETURN -1;
    END IF;

    amountReduced:=0;

    OPEN reductionCursor;

    FETCH reductionCursor INTO theReservation;

    IF NOT FOUND THEN
        RETURN -2;
        END IF;

    LOOP
        IF theReservation.amountPaid>=100 AND amountReduced+5<=maxReduction THEN
            UPDATE Payments
            SET amountPaid=amountPaid-5
            WHERE amountPaid>=100 AND reservationID=theReservation.reservationID;
            amountReduced:=amountReduced+5;
        ELSIF theReservation.amountPaid<100 AND theReservation.amountPaid>=50 AND amountReduced+3<=maxReduction THEN
            UPDATE Payments
            SET amountPaid=amountPaid-3
            WHERE amountPaid<100 AND amountPaid>=50 AND reservationID=theReservation.reservationID;
            amountReduced:=amountReduced+3;
        ELSIF theReservation.amountPaid<50 AND theReservation.amountPaid>=10 AND amountReduced+1<=maxReduction THEN
            UPDATE Payments
            SET amountPaid=amountPaid-1
            WHERE amountPaid<50 AND amountPaid>=10 AND reservationID=theReservation.reservationID;
            amountReduced:=amountReduced+1;
        END IF;
        FETCH reductionCursor INTO theReservation;
        EXIT WHEN NOT FOUND OR amountReduced=maxReduction;
    END LOOP;
    CLOSE reductionCursor;
    RETURN amountReduced;
END
$$ LANGUAGE plpgsql;

