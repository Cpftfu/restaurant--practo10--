#ifndef ACCOUNTANT_H
#define ACCOUNTANT_H
#include "Product.h"
#include "MenuItem.h"
#include "Warehouse.h"
class Accountant
{
public:
	void SubmitProductRequest(const std::string& productName, int quantity);
	void ReceiveProductDelivery(const std::string& productName, int quantity);
	void GenerateFinancialReport(const std::map<std::string, std::shared_ptr<Product>>& products);
	void PrintProductRequests();
	void PrintProductDeliveries();

	static void GetSoldItemsReport();

	static void GenerateSalesReport(const std::map<int, std::shared_ptr<MenuItem>>& menuItems);


	void run();
private:
	std::map<std::string, std::shared_ptr<Product>> products = Warehouse::GetProducts();
	std::map<int, std::shared_ptr<MenuItem>> menuItems;
	std::map<std::string, int> productRequests;
	std::map<std::string, int> productDeliveries;
};

#endif