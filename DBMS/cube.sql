-- CREATE TABLE sales (
--     model VARCHAR(50),
--     year INT,
--     color VARCHAR(20),
--     sales_amount DECIMAL(10, 2)
-- );

-- INSERT INTO sales (model, year, color, sales_amount)
-- VALUES 
-- ('Model A', 2020, 'Red', 5000.00),
-- ('Model A', 2020, 'Blue', 3000.00),
-- ('Model B', 2020, 'Red', 4000.00),
-- ('Model B', 2021, 'Blue', 4500.00),
-- ('Model C', 2021, 'Red', 5500.00),
-- ('Model C', 2021, 'Blue', 6000.00),
-- ('Model A', 2021, 'Red', 7000.00);


-- select NULL, NULL, NULL, SUM(sales_amount)
-- from sales
-- union
-- select model, NULL, NULL, SUM(sales_amount)
-- from sales
-- group by model
-- union
-- select model, year, NULL, SUM(sales_amount)
-- from sales
-- group by model, year
-- union
-- select model, year, color, SUM(sales_amount)
-- from sales
-- group by model, year, color;

-- select model, color, sum(sales_amount) as sales_Count
-- from sales
-- where year=2021
-- group by model,color;
