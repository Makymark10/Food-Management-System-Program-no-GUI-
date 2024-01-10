#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <string.h>
#include <cstring>
#include <algorithm>

using namespace std;

bool adminAccount(string userName, string userPassword){
    ifstream authenticateAccount("authenticateAccount.txt");
    
    if (authenticateAccount.is_open()) {
        string line;
        
        while(getline(authenticateAccount, line)) {
            size_t comma = line.find(",");
            string storedUserName = line.substr(0, comma);
            string storedUserPass = line.substr(comma + 1, line.find(",", comma + 1) - comma - 1);
            
            if (userName == storedUserName && userPassword == storedUserPass) {

                authenticateAccount.close();
                return true;
            }
        }
        authenticateAccount.close();
    }
    else {
        cout << "\tFailed to Execute the file" << endl;
    }
    
    return false;
}

//this function Below, for take order menu Nobugs
void productDeductinTakeOrder(string nameProductInputed,int ProductQuantity) {
    ifstream productStoredDeduct("product_List.txt");
    ofstream tempfile("tempFile.txt");
    
    bool productDeduct = false;
    
    bool productNameFound = false;
    if (productStoredDeduct.is_open()  && tempfile.is_open()) {
        string line;
        productStoredDeduct.clear();
        productStoredDeduct.seekg(0, ios::beg);
        while (getline (productStoredDeduct, line)) {
            size_t pos = line.find(",");
            string storedProductName = line.substr(0, pos);
            string upperCaseProductName;
            transform(storedProductName.begin(), storedProductName.end(), back_inserter(upperCaseProductName), ::toupper);
            //cout << upperCaseProductName << endl;
            //cout << nameProductInputed << endl;
            if(nameProductInputed == upperCaseProductName) {
                line = line.substr(0, line.find(",", pos + 1) + 1) + to_string(ProductQuantity);
                productDeduct = true;
            }
            tempfile << line << endl;
        }
        
        productStoredDeduct.close();
        tempfile.close();
        if(productDeduct) {
            if (remove("product_List.txt") == 0 && rename("tempFile.txt", "product_List.txt") == 0) {
                //cout << "\n\tProduct purchase successful!" << endl;
            }
            else {
                cout << "\n\tUnable to update product list." << endl;
            }   
        }
        else {
            cout << "\n\tUnable to Purchase the product" << endl;
            remove("tempFile.txt");
        }
    } 
    else {
          cout << "\n\tUnable to open files for writing.\n";
    }
    productStoredDeduct.close();
    tempfile.close();
}

void takeOrder() {
    ifstream productList("product_List.txt");
    ofstream tempFile("tempFile.txt");
    
    string line;
    
    if (productList.is_open() && tempFile.is_open()) {
        double productQuantity[1000], totalPrice[1000];
        string nameProducts[100];
        string howManyProducts;
        int howManyProductsInt;
                  
        cout << "\tHow many products: ";
        cin.ignore();
        getline(cin, howManyProducts);
            
        howManyProductsInt = stoi(howManyProducts);
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        for (int i = 1; i <= howManyProductsInt; i++) {
            backtoProductName:
            cout << "\tWhat is the No." << i << " Product: ";
            getline(cin, nameProducts[i]);
               
            productList.clear();
            productList.seekg(0, ios::beg);
        
            bool productFound = false;
            while (getline(productList, line)) {
                  string upperCaseProductInput;
                  transform(nameProducts[i].begin(), nameProducts[i].end(), back_inserter(upperCaseProductInput), ::toupper);
                
                  int productSize = 0, priceSize = 0;
                  size_t comma = line.find(",");
                  string productName = (line.substr(0, comma));
                  string upperCaseProductName;
                  transform(productName.begin(), productName.end(), back_inserter(upperCaseProductName), ::toupper);
                    
                  if (upperCaseProductInput == upperCaseProductName) {
                      productFound = true;
                      break;
                  } 
          }
                
          if (!productFound) {
              cout<< "\tThe Inputed : \"" << nameProducts[i] << "\" is invalid Input!!" << endl;
              goto backtoProductName;
          }
      }     
     // Now you can check if the entered product matches any in the list
     for (int i = 1; i <= howManyProductsInt; i++) {
         productList.open("product_List.txt");
         string upperCaseProductInput;
         transform(nameProducts[i].begin(), nameProducts[i].end(), back_inserter(upperCaseProductInput), ::toupper);
        
         bool productFound = false;
                
         // Rewind the file to the beginning for each product
         productList.clear();
         productList.seekg(0, ios::beg);
         while (getline(productList, line)) {
                int productSize = 0, priceSize = 0;
                size_t comma = line.find(",");
                string productName = (line.substr(0, comma));
                string upperCaseProductName;
                transform(productName.begin(), productName.end(), back_inserter(upperCaseProductName), ::toupper);
                    
                //Convert the string into int and double using the stoi and stod
                double priceProductInt = stod(line.substr(comma + 1, line.find(",", comma + 1) - comma - 1));
                int stockInt = stoi(line.substr(line.find(",", comma + 1) + 1));
                    
                //counting the charaters name of the products
                productSize = productName.length();
                
                // Check if the entered product matches any product in the file
                if (upperCaseProductInput == upperCaseProductName) {
                            
                    productFound = true;
                    backtoProductQuantity:
                    // Perform your logic here
                    cin.ignore();
                    cout << "\tHow many " << nameProducts[i] << " products do you want to buy? ";
                    cin >> productQuantity[i];
                    if (productQuantity[i] <= stockInt) {
                        int stock[1000];
                        //this section will deduct the quantity purchased by the user
                        stockInt -= productQuantity[i];
                        totalPrice[i] = priceProductInt * productQuantity[i];
                        productList.close();
                        tempFile.close();
                        productDeductinTakeOrder(upperCaseProductInput, stockInt);
                        
                        productList.close();
                        tempFile.close();
                        rename("tempFile.txt", "product_List.txt");
                    } 
                    else {
                        cout << "\n\n\tInsuficient Stock!!" << endl;
                        cout << "\tThe Stock only left in " << nameProducts[i] << " is : " << stockInt << "\n" << endl;
                        goto backtoProductQuantity;
                    }
                        
                    break;
                }
                
            }

            // If the product is not found in the file
            if (!productFound) {
                cout << "\tProduct not found: " << nameProducts[i] << endl;
            }
        }
    productList.open("product_List.txt");
    int stopSeconds = 5;
    cout << endl;
    while (true) {
        cout << "\r"<< "\t\t    Processing......." << stopSeconds;
        //cout << " ( Press \'L\' to Lap or press \'Q\' to Exit)";
        cout.flush();
        stopSeconds--;
        if (stopSeconds == 0) {
            break;
        }

        this_thread::sleep_for(1s);
    }
        bool printOutAllProducts = true;
        cout << "\n\n\t*****************Receipt*****************" << endl;
        cout << "\tName: \t\tQuantity \t    Total" << endl;
        if (printOutAllProducts) {
            int productSize = 0;
            string line;
            for (int i = 1; i <= howManyProductsInt ; i++) {
                string upperCaseProductInput;
                transform(nameProducts[i].begin(), nameProducts[i].end(), back_inserter(upperCaseProductInput), ::toupper);
                productList.clear();
                productList.seekg(0, ios::beg);
                while(getline(productList,line)) {
                    size_t comma = line.find(",");
                    string productName = (line.substr(0, comma));
                    productSize = productName.length();
                    string upperCaseProductName;
                    transform(productName.begin(), productName.end(), back_inserter(upperCaseProductName), ::toupper);
                    if(upperCaseProductInput == upperCaseProductName) {
                        productSize = productName.length();
                    
                        if (productSize > 6) {
                            cout<< "\t" << productName << "\t   " << productQuantity[i] << "\t\t    " << totalPrice[i] << endl;
                        }
                        else {
                            cout<< "\t" << productName << "\t\t   " << productQuantity[i] << "\t\t    " << totalPrice[i] << endl;
                        }
                    }
                }   
            }
            printOutAllProducts = false;
        }
    
    }
    productList.close();
    tempFile.close();
}

string productsList(){ 
    fstream productList("product_List.txt", ios::out|ios::in);
    
    if (productList.is_open()) {
        string line;
        
        while (getline(productList, line)) {
            int productSize = 0, priceSize = 0;
            size_t comma = line.find(",");
            string productName = line.substr(0, comma);
            string priceProductString = line.substr(comma + 1, line.find(",", comma + 1) - comma - 1);
            size_t comma2 = line.find(",", comma + 1);
            string stockSrting = line.substr(comma2 + 1, line.find(",", comma2 + 1) - comma2 - 1);
            
            //Convert the string into int using the stoi
            
            double priceProductInt = stod(line.substr(comma + 1, line.find(",", comma + 1) - comma - 1));
            int stockInt = stoi(line.substr(comma2 + 1, line.find(",", comma2 + 1) - comma2 - 1));
            
            //counting the charaters name of the products
            productSize = productName.length();
            priceSize = priceProductString.length();
            if (productSize >= 8) {
                cout<< "\t" << productName << "\t\t\t " << priceProductInt << endl;
            } 
            else if(priceSize > 10) {
                cout<< "\t" << productName << "\t\t\t" << priceProductInt << endl;
            }
            else {
                cout<< "\t" << productName << "\t\t\t\t " << priceProductInt << endl;
            }
        }
        //AFTER EXECUTING THE LIST OF THE PRODUCT I WILL ECUTE THE FUNCTION HERE TO DO MAGIC ONE HEHEHE
        productList.close();
    }
    else {
        cout << "\tFailed to Execute the file" << endl;
    }
    productList.close();
}
//this function Above, for take order menu Nobugs


//this code below, for the add new product or update the existing products
void updateExistProduct(string nameProductInputed, double priceInputed, int quantityInputed) {
    ifstream inputFile("product_List.txt");
    ofstream tempFile("tempFile.txt");

    bool quantityUpdated = false;
    bool priceUpdated = false;

    if (inputFile.is_open() && tempFile.is_open()) {
        string line;
        while (getline(inputFile, line)) {
            size_t pos = line.find(",");
            string storedProductName = line.substr(0, pos);
            string upperCaseProductName;
            transform(storedProductName.begin(), storedProductName.end(), back_inserter(upperCaseProductName), ::toupper);

            if (nameProductInputed == upperCaseProductName) {
                // Update quantity
                line = line.substr(0, line.find(",", pos + 1) + 1) + to_string(quantityInputed);
                quantityUpdated = true;

                // Update price
                line = line.substr(0, pos + 1) + to_string(priceInputed) + line.substr(line.find(",", pos + 1));
                priceUpdated = true;
            }

            tempFile << line << endl;
        }

        inputFile.close();
        tempFile.close();

        if (quantityUpdated || priceUpdated) {
            if (remove("product_List.txt") == 0 && rename("tempFile.txt", "product_List.txt") == 0) {
              //  cout << "\n\tProduct update successful!" << endl;
            } else {
                cout << "\n\tUnable to update product list." << endl;
            }
        } else {
            cout << "\n\tProduct not found for updating." << endl;
            remove("tempFile.txt");
        }
    } else {
        cout << "\n\tUnable to open files for writing.\n";
    }
}

void addProduct(string nameProductInputed, double priceInputed, int quantityInputed) {
    ifstream inputFile("product_List.txt");
    ofstream tempFile("tempFile.txt");

    bool productFound = false;

    if (inputFile.is_open() && tempFile.is_open()) {
        string line;
        while (getline(inputFile, line)) {
            size_t pos = line.find(",");
            string storedProductName = line.substr(0, pos);
            string upperCaseProductName;
            transform(storedProductName.begin(), storedProductName.end(), back_inserter(upperCaseProductName), ::toupper);

            if (nameProductInputed == upperCaseProductName) {
                // Update quantity and price
                //line = nameProductInputed + "," + to_string(quantityInputed) + "," + to_string(priceInputed);
                productFound = true;
            }

            tempFile << line << endl;
        }

        inputFile.close();

        if (!productFound) {
            // Product not found, add a new entry
            tempFile << nameProductInputed << "," << to_string(priceInputed) << "," << to_string(quantityInputed) << endl;
        }

        tempFile.close();

        // Replace the old file with the updated one
        if (remove("product_List.txt") == 0 && rename("tempFile.txt", "product_List.txt") == 0) {
           //cout << "\n\tProduct update successful!" << endl;
        } else {
            cout << "\n\tUnable to update product list." << endl;
        }
    } else {
        cout << "\n\tUnable to open files for writing.\n";
    }
}

string addNewProducts() {
    ifstream addNewProduct("product_List.txt");
    
    if (addNewProduct.is_open()) {
        choice:
        string line;
        string addNewProductsName[1000];
        double addNewProductsPrice[1000];
        int addNewProductsStock[1000];
        int howManyProductsAdd;
        
        
        cout << "\n\n\tHow Many Product(s) You want to add : ";
        cin >> howManyProductsAdd;
    
        if (howManyProductsAdd == 0) {
            cout << "\n\tOK!!" << endl;
        }
        else if(howManyProductsAdd < 0) {
            cout << "\n\tOK!!" << endl;
        }
        else {
            
            for (int i = 1; i <= howManyProductsAdd; i++) {
                cout << "\n\tWhat is the No." << i << " Product Name : ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, addNewProductsName[i]);
                
                
                addNewProduct.clear();
                addNewProduct.seekg(0, ios::beg);
                
                bool productFound = false;
                bool productFoundNot = false;
                while (getline(addNewProduct, line)) {
                    
                    string upperCaseProductInput;
                    transform(addNewProductsName[i].begin(), addNewProductsName[i].end(), back_inserter(upperCaseProductInput), ::toupper);
                
                    int productSize = 0, priceSize = 0;
                    size_t comma = line.find(",");
                    string productName = (line.substr(0, comma));
                    string upperCaseProductName;
                    transform(productName.begin(), productName.end(), back_inserter(upperCaseProductName), ::toupper);
                    
                    //Convert the string into int and double using the stoi and stod
                    double priceProductInt = stod(line.substr(comma + 1, line.find(",", comma + 1) - comma - 1));
                    int stockInt = stoi(line.substr(line.find(",", comma + 1) + 1));
                    
                    
                    if (upperCaseProductInput == upperCaseProductName) {
                        cout << "\n\tHow much the price of the " << addNewProductsName[i] << " : ";
                        cin >> addNewProductsPrice[i];
                        cout << "\n\tHow many the pieces of the " << addNewProductsName[i] << " : ";
                        cin>> addNewProductsStock[i];
                    
                        char choice1, choice2;
                        cout << "\n\tThe Inputed : \"" << addNewProductsName[i] << "\" Has already stored in the server!!\n" << endl;
                        cout << "\tDo you want to store both price and the stock? Y/N : ";
                        cin >> choice1;
                    
                        if (choice1 == 'y' || choice1 == 'y') {
                            addNewProduct.close();
                            updateExistProduct(upperCaseProductInput, addNewProductsPrice[i], addNewProductsStock[i]);
                            productFound = true;
                            break;
                        } else if(choice1 == 'n' || choice1 == 'N') {
                            cout << "\tDo you only want to store the The stock of the : " << productName << "? Y/N: ";
                            cin >> choice2;
                            
                            if (choice2 == 'y'|| choice2 == 'Y') {
                                addNewProduct.close();
                                updateExistProduct(upperCaseProductInput, priceProductInt, addNewProductsStock[i]);
                                productFound = true;
                                break;
                            }
                            else if (choice2 == 'n' || choice2 == 'N') {
                                addNewProduct.close();
                                updateExistProduct(upperCaseProductInput, addNewProductsPrice[i], stockInt);
                                productFound = true;
                                break;
                            }
                            else {
                                cout << "\tInvalid Input : " << choice2 << endl;
                            }
                        } 
                        else {
                            cout << "\tInvalid Input : " << choice1 << endl;
                        }
                        
                    }
                }
                
                //below of this line of code, the if statement will sotre the data of the new product(s)
                if (!productFound) {
                    //CONVERT THE FIRST LETTER TO UPPERCASE
                    addNewProductsName[i].front() = toupper(addNewProductsName[i].front());
                    
                    cout << "\n\tHow much the price of the " << addNewProductsName[i] << " : ";
                    cin >> addNewProductsPrice[i];
                    cout << "\n\tHow many the pieces of the " << addNewProductsName[i] << " : ";
                    cin>> addNewProductsStock[i];
                
                    //BELOW THIS CODE THE COUT IS TO CHECK IF THE ADDNEWPRODUCT CONVERT THE FIRST TO LETTER TO UPPERCASE
                    //cout << "\t" << addNewProductsName[i] << endl;
                    addNewProduct.close();
                    addProduct(addNewProductsName[i], addNewProductsPrice[i], addNewProductsStock[i]);
                }
                
            }
            int stopSeconds = 5;
            //i will put comthing here if ia have time to do it
            
            
            cout << endl;
            while (true) {
                cout << "\r"<< "\t\t    Processing......." << stopSeconds;
                //cout << " ( Press \'L\' to Lap or press \'Q\' to Exit)";
                cout.flush();
                stopSeconds--;
                if (stopSeconds == -1) {
                    cout << "\n\n\t\t  Succesfully Processed\n" << endl;
                    break;
                }

                this_thread::sleep_for(1s);
            }
            
            char choices;
            cout << "\tDo You Want tp add more Products? Y/N : ";
            cin >> choices;
            
            if (choices == 'y'  || choices == 'Y') {
                goto choice;
            }
             
            }
        addNewProduct.close();
    }
    else {
        cout << "The file has not been found" << endl;
        cout << "Please Insert the FILE" << endl;
    }
    addNewProduct.close();
}
//this code above, for the add new product or update the existing products

//this code below, for the add new product or update the product Aviable and Update the Stock
void updateProductAvailableStock(string nameProductInputed, int quantityInputed) {
    ifstream inputFile("product_List.txt");
    ofstream tempFile("tempFile.txt");

    bool quantityUpdated = false;
    bool priceUpdated = false;

    if (inputFile.is_open() && tempFile.is_open()) {
        string line;
        while (getline(inputFile, line)) {
            size_t pos = line.find(",");
            string storedProductName = line.substr(0, pos);
            string upperCaseProductName;
            transform(storedProductName.begin(), storedProductName.end(), back_inserter(upperCaseProductName), ::toupper);

            if (nameProductInputed == upperCaseProductName) {
                // Update quantity
                line = line.substr(0, line.find(",", pos + 1) + 1) + to_string(quantityInputed);
                quantityUpdated = true;
            }

            tempFile << line << endl;
        }

        inputFile.close();
        tempFile.close();

        if (quantityUpdated || priceUpdated) {
            if (remove("product_List.txt") == 0 && rename("tempFile.txt", "product_List.txt") == 0) {
              //  cout << "\n\tProduct update successful!" << endl;
            } else {
                cout << "\n\tUnable to update product list." << endl;
            }
        } else {
            cout << "\n\tProduct not found for updating." << endl;
            remove("tempFile.txt");
        }
    } else {
        cout << "\n\tUnable to open files for writing.\n";
    }
}

string productAvailableStock() {
    ifstream productAvailableStocks("product_List.txt", ios::in|ios::out);
    int stopSeconds = 5;
    //i will put comthing here if ia have time to do it
            
    cout << endl;
    while (true) {
        cout << "\r"<< "\t\t Retrieving Data......." << stopSeconds;
        //cout << " ( Press \'L\' to Lap or press \'Q\' to Exit)";
        cout.flush();
        stopSeconds--;
        if (stopSeconds == -1) {
            cout << "\n\n\t**************Products Stock**************" << endl; 
            cout << "\tProduct\t\t\t    Availabe Stock" << endl;
            break;
        }

        this_thread::sleep_for(1s);
    }
    
    if (productAvailableStocks.is_open()) {
        string line;
        
        while (getline(productAvailableStocks, line)) {
            int productSize = 0, priceSize = 0;
            size_t comma = line.find(",");
            string productName = line.substr(0, comma);
            string priceProductString = line.substr(comma + 1, line.find(",", comma + 1) - comma - 1);
            size_t comma2 = line.find(",", comma + 1);
            string stockSrting = line.substr(comma2 + 1, line.find(",", comma2 + 1) - comma2 - 1);
            
            //Convert the string into int using the stoi
            
            double priceProductInt = stod(line.substr(comma + 1, line.find(",", comma + 1) - comma - 1));
            int stockInt = stoi(line.substr(comma2 + 1, line.find(",", comma2 + 1) - comma2 - 1));
            
            //counting the charaters name of the products
            productSize = productName.length();
            priceSize = priceProductString.length();
            if (productSize >= 8) {
                //cout << priceSize << endl;
                if (stockInt == 0) {
                    cout<< "\t" << productName << "\t\t     Out of Stock" << endl;
                }
                else {
                   cout<< "\t" << productName << "\t\t\t  " << stockInt << endl;     
                }
                
            } 
            else if(priceSize > 10) {
                if (stockInt == 0) {
                    cout<< "\t" << productName << "\t\t\t Out of Stock" << endl;
                }
                else {
                    cout<< "\t" << productName << "\t\t\t " << stockInt << endl;
                }
            }
            else {
                //cout << priceSize << endl;
                 //cout << productSize << endl;
                if (stockInt == 0) {
                    cout<< "\t" << productName << "\t\t\t     Out of Stock" << endl;
                }
                else {
                    cout<< "\t" << productName << "\t\t\t\t  " << stockInt << endl;
                }
                //cout << stockSrting << endl;
            }
        }
        char choices;
        cout << "\n\n\tDo You want to Update the Product Stock? Y/N : ";
        cin >> choices;
        
        if (choices == 'y' || choices == 'Y') {
            bool updateStock = true;
            while (updateStock) {
                int backup;
                long int QuantityStock[1000], totalStock[1000];
                string nameProducts[100];
                string howManyProducts;
                int howManyProductsInt;
    
                cout << "\tHow many products do you Wanted to update their Stock: ";
                cin.ignore();
                getline(cin, howManyProducts);
            
                howManyProductsInt = stoi(howManyProducts);
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                for (int i = 0; i < howManyProductsInt; i++) {
                    backtoProductName:
                    cout << "\tWhat is the No." << i + 1 << " Product: ";
                    getline(cin, nameProducts[i]);
               
                    productAvailableStocks.clear();
                    productAvailableStocks.seekg(0, ios::beg);
                
                    bool productFound = false;
                    while (getline(productAvailableStocks, line)) {
                    
                        string upperCaseProductInput;
                        transform(nameProducts[i].begin(), nameProducts[i].end(), back_inserter(upperCaseProductInput), ::toupper);
                    
                        int productSize = 0, priceSize = 0;
                        size_t comma = line.find(",");
                        string productName = (line.substr(0, comma));
                        string upperCaseProductName;
                        transform(productName.begin(), productName.end(), back_inserter(upperCaseProductName), ::toupper);
                    
                        if (upperCaseProductInput == upperCaseProductName) {
                            productFound = true;
                            break;
                        } 
                    }
                
                    if (!productFound) {
                        cout<< "\tThe Inputed : \"" << nameProducts[i] << "\" is invalid Input!!" << endl;
                    
                        goto backtoProductName;
                    }
                }
            
            
                // Now you can check if the entered product matches any in the list
                for (int i = 0; i < howManyProductsInt; i++) {
                    productAvailableStocks.open("product_List.txt");
                    string upperCaseProductInput;
                    transform(nameProducts[i].begin(), nameProducts[i].end(), back_inserter(upperCaseProductInput), ::toupper);
            
                    bool productFound = false;
                
                    // Rewind the file to the beginning for each product
                    productAvailableStocks.clear();
                    productAvailableStocks.seekg(0, ios::beg);
                    while (getline(productAvailableStocks, line)) {
                
                        int productSize = 0, priceSize = 0;
                        size_t comma = line.find(",");
                        string productName = (line.substr(0, comma));
                    
                        //Convert the string into int and double using the stoi and stod
                        double priceProductInt = stod(line.substr(comma + 1, line.find(",", comma + 1) - comma - 1));
                        int stockInt = stoi(line.substr(line.find(",", comma + 1) + 1));
                        string upperCaseProductName;
                        transform(productName.begin(), productName.end(), back_inserter(upperCaseProductName), ::toupper);
                    
                        //counting the charaters name of the products
                        productSize = productName.length();
                        //priceSize = priceProductString.length();
                        
                        //THIS SECTION HAS A BUG THAT CANNOT UPDATE THE OLD VALUE OF STOCK
                        // Check if the entered product matches any product in the file
                        if (upperCaseProductInput == upperCaseProductName) {
                        
                            productFound = true;
                            backtoProductQuantity:
                            
                            backto:
                            cin.ignore();
                            cout << "\n\tHow many " << nameProducts[i] << " do you wanted to update their Stock : ";
                            cin >> QuantityStock[i];
                            
                            //test if the data retreining data is accurate 
                            //cout << productName << "," << priceProductInt << "," << totalStock[i];
                            
                            if (QuantityStock[i] > 10000000) {
                                
                                //test if the data retreining data is accurate 
                                //cout << productName << "," << priceProductInt << "," << stockInt;
                                
                                cout << "\n\tCannot Handle the Value!! \n" << endl;
                                goto backto;
                            }
                            else {
                                char pick1;
                            
                                cout << "\n\tDo you want to Add the the " << QuantityStock[i] << " to the existing stock? Y/N: ";
                                cin >> pick1;
                                
                                if (pick1 == 'y' || pick1 == 'Y') {
                                    totalStock[i] = stockInt + QuantityStock[i];
                                    productAvailableStocks.close();
                                    updateProductAvailableStock(upperCaseProductInput, totalStock[i]);
                                } 
                                else if (pick1 == 'n' || pick1 == 'N') {
                                    productAvailableStocks.close();
                                    updateProductAvailableStock(upperCaseProductInput, QuantityStock[i]);
                                }
                                else {
                                    cout << "\n\tThe Inputed : \"" << pick1 << "\" is INVALID\n" << endl; 
                                }
                            }        
                        }
                    }

                    // If the product is not found in the file
                    if (!productFound) {
                        cout << "\tProduct not found: " << nameProducts[i] << endl;
                        // You can handle this case as needed
                    }
                }
                break;
                
                int stopSeconds = 5;
                //i will put comthing here if ia have time to do it
            
            
                cout << endl;
                while (true) {
                    cout << "\r"<< "\t\t    Processing......." << stopSeconds;
                    //cout << " ( Press \'L\' to Lap or press \'Q\' to Exit)";
                    cout.flush();
                    stopSeconds--;
                    if (stopSeconds == -1) {
                        cout << "\n\n\t\t  Succesfully Processed\n" << endl;
                        break;
                    }

                this_thread::sleep_for(1s);
                }
                
            }
            
        }
        productAvailableStocks.close();
    }
    else {
        cout << "The file has not been found" << endl;
        cout << "Please Insert the FILE" << endl;
    }
    productAvailableStocks.close();
}
//this code above, for the add new product or update the product Aviable and Update the Stock

//this code below, for the removing a specifc line 
void removeProductExist(string nameProductInputed) {
    ifstream inputFile("product_List.txt");
    ofstream tempFile("tempFile.txt");

    bool quantityUpdated = false;
    bool priceUpdated = false;

    if (inputFile.is_open() && tempFile.is_open()) {
        string line;
        while (getline(inputFile, line)) {
            size_t pos = line.find(",");
            string storedProductName = line.substr(0, pos);
            string upperCaseProductName;
            transform(storedProductName.begin(), storedProductName.end(), back_inserter(upperCaseProductName), ::toupper);

            if (nameProductInputed != upperCaseProductName) {
                tempFile << line << endl;
            }
        }

        inputFile.close();
        tempFile.close();
        if (remove("product_List.txt") == 0 && rename("tempFile.txt", "product_List.txt") == 0) {
            //  cout << "\n\tProduct update successful!" << endl;
        }
        else {
            cout << "\n\tUnable to update product list." << endl;
        }
        
    } else {
        cout << "\n\tUnable to open files for writing.\n";
    }
}

string removeExistProduct() {
    ifstream productList("product_List.txt");
    
    if (!productList.is_open()) {
        cout << "\n\n\tCannot find the FIlE" << endl;
    }
    else {
        string line;
        int stopSeconds = 5;
        //i will put comthing here if ia have time to do it
                
        cout << endl;
        while (true) {
            cout << "\r"<< "\t\t   Retrieving Data......." << stopSeconds;
            //cout << " ( Press \'L\' to Lap or press \'Q\' to Exit)";
            cout.flush();
            stopSeconds--;
            if (stopSeconds == -1) {
                break;
            }

            this_thread::sleep_for(1s);
        }
        
        
        int howManyProduct = 0;
        cout << "\n\n\t    **********Remove the Product**********" <<  endl;
        
        cout << "\n\tHow many product(s) do you wanted to remove in the file : ";
        cin >> howManyProduct;
        
        string productName[howManyProduct];
        
        if (howManyProduct == 1) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            bak:
            cout << "\n\tWhat is the name of the product : ";
            getline(cin, productName[0]);
            
            string upperCaseProductInput;
            transform(productName[0].begin(), productName[0].end(), back_inserter(upperCaseProductInput), ::toupper);
            bool productFound = false;
                
            productList.clear();
            productList.seekg(0, ios::beg);
            while (getline(productList, line)) {
                size_t comma = line.find(",");
                string storedProductName = line.substr(0, comma);
                string priceProductString = line.substr(comma + 1, line.find(",", comma + 1) - comma - 1);
                size_t comma2 = line.find(",", comma + 1);
                string stockSrting = line.substr(comma2 + 1, line.find(",", comma2 + 1) - comma2 - 1);
                    
                string upperCaseProducStored;
                transform(storedProductName.begin(), storedProductName.end(), back_inserter(upperCaseProducStored), ::toupper);
        
                if (upperCaseProductInput == upperCaseProducStored) {
                    productList.close();
                    removeProductExist(upperCaseProductInput);
                    productFound = true;
                    int stopSeconds = 5;
                    cout << "\n\n";
                    while (true) {
                        cout << "\r"<< "\t\t   Processing......." << stopSeconds;
                        //cout << " ( Press \'L\' to Lap or press \'Q\' to Exit)";
                        cout.flush();
                        stopSeconds--;
                        if (stopSeconds == -1) {
                            cout << "\n\n";
                            cout << "\tProduct Name: " << productName[0] << ", Successfully remove" << endl;
                        break;
                    }

                this_thread::sleep_for(1s);
            }
                    
                    break;
                }
            }
            if(!productFound) {
                cout << "\n\tThe Inputed : \"" << productName[0] << "\" not Found" << endl;
                goto bak;
            }
        }
        else if (howManyProduct > 1) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            for (int i = 1; i <= howManyProduct; i++) {
                backtoName:
                productList.open("product_List.txt");
                cout << "\n\tWhat is the No." << i << " Name of the product : ";
                getline(cin, productName[i]);
                
                
                string upperCaseProductInput;
                transform(productName[i].begin(), productName[i].end(), back_inserter(upperCaseProductInput), ::toupper);
                bool productFound = false;
                
                productList.clear();
                productList.seekg(0, ios::beg);
                while (getline(productList, line)) {
                    size_t comma = line.find(",");
                    string storedProductName = line.substr(0, comma);
                    string priceProductString = line.substr(comma + 1, line.find(",", comma + 1) - comma - 1);
                    size_t comma2 = line.find(",", comma + 1);
                    string stockSrting = line.substr(comma2 + 1, line.find(",", comma2 + 1) - comma2 - 1);
                    
                    string upperCaseProducStored;
                    transform(storedProductName.begin(), storedProductName.end(), back_inserter(upperCaseProducStored), ::toupper);
                    if (upperCaseProductInput == upperCaseProducStored) {
                        productList.close();
                        removeProductExist(upperCaseProductInput);
                        productFound = true;
                        break;
                    }
                }
                if(!productFound) {
                    cout << "\n\tThe Inputed : \"" << productName[i] << "\" not Found" << endl;
                    goto backtoName;
                }
            }
            int stopSeconds = 5;
            cout << "\n\n";
            while (true) {
                cout << "\r"<< "\t\t   Processing......." << stopSeconds;
                //cout << " ( Press \'L\' to Lap or press \'Q\' to Exit)";
                cout.flush();
                stopSeconds--;
                if (stopSeconds == -1) {
                    cout << "\n\n";
                    for (int i = 1;i <= howManyProduct; i++) {
                        cout << "\tProduct Name: " << productName[i] << ", Successfully remove" << endl;
                    }
                    break;
                }

                this_thread::sleep_for(1s);
            }
        }
    }
}
//this code above, for the removing a specifc line 

int main()
{
    /* int stopSeconds = 5;
    while (true) {
        cout << "\r"<< "\t\t Retrieving the Data......." << stopSeconds;
        //cout << " ( Press \'L\' to Lap or press \'Q\' to Exit)";
        cout.flush();
        stopSeconds--;
        if (stopSeconds == -1) {
            break;
        }
    
        this_thread::sleep_for(1s);
    }
    */
    string choices;
    string userName = "admin", userPass = "admin123";        
    cout << "\n\n\t\t \"Food Management System\"\n" << endl;
   
    signloop:
    /*cout << "\t AdminAccount" << endl;
    cout << "\t Enter Username: ";
    cin >> userName;
    cout << "\t Enter Password: ";
    cin >> userPass;
    */
    if (adminAccount(userName, userPass)) {
        choices:
            
        cout << "\n\n\t\t \"Food Management System\"\n" << endl;
        cout << "\n\tPick a Task" << endl;
        cout << "\t(1) Take a Order" << endl;
        cout << "\t(2) Add new Product" << endl;
        cout << "\t(3) Look the the avaible stock" << endl;
        cout << "\t(4) Remove Products" << endl;
        cout << "\t(5) Exit" << endl;
        cout << "\tType Here: ";
        cin >> choices;
               
        if (choices == "1") {
                    
            cout << "\n\n";
            int stopSeconds = 5;
            while (true) {
                cout << "\r"<< "\t      Retrieving the Data......." << stopSeconds;
                //cout << " ( Press \'L\' to Lap or press \'Q\' to Exit)";
                cout.flush();
                stopSeconds--;
                if (stopSeconds == -1) {
                    break;
                }
    
                this_thread::sleep_for(1s);
            }
            
            cout << "\n\n\t*****************MENU*****************" << endl; 
            cout << "\tProduct\t\t\t\t Price" << endl;
            productsList();
            takeOrder();
            goto choices;
        }
        else if (choices == "2") {
            addNewProducts();
            goto choices;
        }
        else if (choices == "3") {
            productAvailableStock();
            goto choices;
        }
        else if (choices == "4") {
            removeExistProduct();
            goto choices;
        } 
        else if (choices == "5") {
            cout << "\n\n\tFOOD MANAGEMENT SYSTEM V1.0" << endl;
            cout << "\tCreate By: Mark Joero" << endl;
        } 
        else {
            cout << "\n\tInputed : \"" << choices << "\" is INVALID\n\n";
            goto choices;
        }
                
    } 
    else {
        cout << "\t Wrong Username of Password\n" << endl;
        goto signloop;
    }
    backtologin:
    string backtologin;
    cout << "\n\tDo you want to try agian the FOOD MANAGEMENT SYSTEM V1.0 ? Y/N ";
    cin >> backtologin;
    
    if (backtologin == "y" || backtologin == "Y") {
        goto signloop;
    } 
    else if (backtologin == "n" || backtologin == "N") {
        cout << endl;
    } else {
        cout << "\tInputed : \"" << backtologin << "\" is INVALID\n\n" << endl; 
        goto backtologin;
        
        cout << endl;
    }
} 

