SELECT DISTINCT C.name, C.level, C.joinDate
FROM Customers C, Vehicles V
WHERE C.name LIKE '_e%' 
    AND (C.custID=V.ownerID AND V.color='R') 
    ORDER BY Name DESC, joinDate ASC