# Calculator

## Intro
    Этот проект представляет расширенную версию стандартного калькулятора.
    Проект выполнен на С++ с использованием фреймворка Qt.
    При реализации использован архитектурный паттерн MVC.git branch 

## Features
- ввод произвольного математического выражения с клавиатуры
- построение графика функции, заданной с помощью выражения с переменной x
- расчет по кредиту
- расчет по вкладу
    
## Figures

### Отрисовка функции y = sqrt(cos(x))*cos(200*x)+sqrt(x^2)-3.1415/4*(4-x^2)^0.01
![PlottingOne](misc/images/PlottingOne.png)

### Отрисовка функции y = sqrt(sin(x)^2)+5*2.71^(-(x^100))*cos(x)
![PlottingTwo](misc/images/PlottingTwo.png)

### Расчет аннуитетных выплат по кредиту
![CreditAnnuity](misc/images/CreditAnnuity.png)

### Расчет дифференцированных выплат по кредиту
![CreditDiff](misc/images/CreditDiff.png)

### Расчет по вкладу без капитализации
![DepositOne](misc/images/DepositOne.png)

### Расчет по вкладу с капитализацией
![DepositCapitalization](misc/images/DepositCapitalization.png)

## Notes
- Результаты расчетов по кредитам и вкладам сравнивались с аналогичными на сайте *calcus.ru*