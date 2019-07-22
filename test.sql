CREATE TABLE Flights (
    flight varchar(20),
    dep varchar(3),
    des varchar(3),
    airline varchar(3),
    distance int,
    model varchar(5),
    departure_year smallint,
    departure_month tinyint,
    departure_date tinyint,
    departure_hour tinyint,
    departure_min tinyint,
    arrival_year smallint,
    arrival_month tinyint,
    arrival_date tinyint,
    arrival_hour tinyint,
    arrival_min tinyint,
    stops tinyint,
    F tinyint,
    FL tinyint,
    C tinyint,
    CL tinyint,
    Y tinyint,
    YL tinyint,
    B tinyint,
    BL tinyint,
    K tinyint,
    KL tinyint,
    M tinyint,
    ML tinyint
);

INSERT INTO Flights (flight, dep, des, airline, distance, model, departure_year, departure_month, departure_date,
    departure_hour, departure_min, arrival_year, arrival_month, arrival_date, arrival_hour,
    arrival_min,stops, F, FL, C, CL, Y, YL, B, BL, K, KL, M, ML)
VALUES ('CA155', 'PEK', 'SHA', 'CA', 1213, '767', 2019, 7, 25, 7, 45, 2019, 7, 25, 9, 50, 0, 10, 0, 10, 0, 20, 0, 20, 0, 20 ,0, 20, 0);

INSERT INTO Flights (flight, dep, des, airline, distance, model, departure_year, departure_month, departure_date,
    departure_hour, departure_min, arrival_year, arrival_month, arrival_date, arrival_hour,
    arrival_min,stops, F, FL, C, CL, Y, YL, B, BL, K, KL, M, ML)
VALUES ('CA0721', 'PEK', 'SHA', 'CA', 1213, '767', 2019, 7, 25, 10, 0, 2019, 7, 25, 12, 5, 0, 10, 0, 10, 0, 20, 0, 20, 0, 20, 0, 20, 0);


UPDATE Flights SET F = 10, FL = 0 WHERE flight = "CA155";
