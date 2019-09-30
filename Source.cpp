#include<iostream>
#include<string>
#include<cmath>
#include<algorithm>
#include <vector>
using namespace std;
struct minterm {                         // a struct for the minterm input by the user 
	int index = 0;                       // the user input (the number of the minterm)
	int ones = 0;                        // the number of ones in the minterm
	int include = 0;                     // the number of primes that include this minterm
	string binary = "";                  // the binary form of the minterm
	bool dontcare = false;              // if it's a dont care or not
	bool includef = false;              // if it's included in the optimized function or not
	bool operator < (const minterm & rhs) const {
		return (ones < rhs.ones);
	}
};
struct forms {        // a struct for the minterms in the cols
	string form;      // the binary form of the merged minterms
	bool ticked = false;  // if it's ticked or not
	int ones = 0;          // the number of ones
};
struct prime {           // a struct for the prime implicants
	string form;         // the binary form
	string min;          // the variable form; for instance : AC'D
	bool mark = false;    // marked as essential or not
	bool care = false;    // includes minterms or not (not only don't cares)
	int included = 0;    // the number of minterms included in the coverage table
	bool includedf = false;  // whether it's included in the optimized function or not 
	bool operator < (const prime & rhs) const {
		return (included < rhs.included);
	}
	};

	void verify();
	void Input();
	void convert();
	string DtoB(int, int);
	string fillinzeros(string);
	void countones();
	bool differences(string, string);
	string showDiff(string, string);
	void getprime();
	void comparison(vector<forms>);
	void compare();
	void deleteprime();
	void ConvertToVariables();
	void diplayfunction();
	void getessential();
	bool checkexist(int, string);
	void clearcover();
	void cover();
	void createcoverage();
	void printfunction();
	void header();

	vector<prime> primes;    //containing all the prime implicants
	vector<forms> cola;      // containing the data of an imaginary col
	vector<forms> colb;      // containing the data of an imaginary col 
	vector<minterm> F;       // contains the data of the minterms (the user input)
	int n;                   // the number of variables in the function
	bool checknotzero = false;    //whether the function is zero or not

	void main()
	{


		cout << "Please enter the number of variables in the function" << endl;
		cin >> n;
		while (n < 1)                                                                  //Check whether the number of variables is reasonable or not. 
		{
			cout << "Invalid input; Please re-enter the number of variables in the function" << endl;
			cin >> n;
		}
		Input();                                                                        // Input the minterms and the don't care.
		header();
		if (F.size() == pow(2, n))                                                       // Check if the function is always 1.
		{
			cout << "The function is always 1" << endl;
		}
		else
			if ((F.size() != 0) && checknotzero)                                        // check if the function is zero or not
			{
				verify();                                                               // Validate and verify the given inputs.
				convert();                                                              // conversion to binary
				countones();                                                            // counting the ones in the binary number.
				sort(F.begin(), F.end());                                               // sorting based on the number of ones.
				compare();                                                              // Creating the table and starting the tabulation method.
				deleteprime();                                                          // deleting the repeated primes.
				ConvertToVariables();                                                   // converting the binary form into variables: A,B,C,...
				getessential();                                                         // Extracting the essential primes.
				header();
				diplayfunction();                                                       // Displaying the prime implicants and then the essential impiclants.
				createcoverage();                                                       // Create the coverage table and extracts the optmization of the function
				header();
				printfunction();                                                        // Displays the optimized function
			}
			else
				cout << "The function is always zero" << endl;
		header();
		system("pause");
	}

	void Input()
	{
		minterm g;                               // A temporary storage for the input.
		cout << "Please Enter the minterms first, if you finish insert -1" << endl;
		cin >> g.index;
		while (g.index != -1)
		{
			F.push_back(g);                     // Storing the input into the main function -adding new minterms-
			cin >> g.index;

		}
		if (F.size() != 0)                     // Checking the existance of minterms, otherwise the function will be zero
			checknotzero = true;
		cout << "Please Enter the don't cares, if you finish insert -1" << endl;
		cin >> g.index;
		while (g.index != -1)
		{
			g.dontcare = true;                // Marking the don't cares as don't care minterms
			F.push_back(g);					// Storing the input into the main function -adding new don't cares-	
			cin >> g.index;
		}
	}

	void verify()
	{
		int ch = 0;                        //Stores the choice of the user, whether to change or delete the minterm(/or don't care)
		for (int i = 0; i < F.size(); i++)
		{
			if (F[i].index < 0)             // Checking if a minterm index has a negative value;
			{
				cout << "Invalid input of minterm " << F[i].index << " ; it's Negative; Press 1 to delete or 2 to change" << endl;
				cin >> ch;
				while ((ch != 1) && (ch != 2))    //Validates the choice input
				{
					cout << "Invalid Input ... Please press 1 to delete or 2 to change" << endl;
					cin >> ch;
				}
			}
			else
				if (F[i].index >= pow(2, n))              // Checking if a minterm is greater than the possible limit of minterms 2^n where n is the number of variables
				{
					cout << "Invalid input of minterm " << F[i].index << " ; it's greater than 2^n; Press 1 to delete or 2 to change" << endl;
					cin >> ch;
					while ((ch != 1) && (ch != 2))    //Validates the choice input
					{
						cout << "Invalid Input ... Please press 1 to delete or 2 to change" << endl;
						cin >> ch;
					}
				}
				else
					for (int j = i + 1; j < F.size(); j++)
					{

						if (F[i].index == F[j].index)               //Checking if a minterm is repeated
						{
							cout << "Invalid input of minterm " << F[j].index << " ; it's repeated; Press 1 to delete or 2 to change" << endl;
							cin >> ch;
							while ((ch != 1) && (ch != 2))    //Validates the choice input
							{
								cout << "Invalid Input ... Please press 1 to delete or 2 to change" << endl;
								cin >> ch;
							}
						}
					}
			if (ch == 1)   //if the choice input is 1, the minterm is deleted with all its recorded data 
			{
				F.erase(F.begin() + i);
				cout << "Deleted" << endl;
				ch = 0;
			}
			else
				if (ch == 2)       //if the choice input is 2, the minterm/don'tcare is replaced with a new input value
				{
					cout << "Please insert the new value" << endl;
					cin >> F[i].index;
					ch = 0;
					verify();
				}	
		}
	}
	void convert()            // conversion from decimal to binary for all the minterms. 
	{
		for (int i = 0; i < F.size(); i++)
		{
			F[i].binary = DtoB(F[i].index, n);     // calls a function that converts the index input by the user into a binary value and stores it in the main function struct.
		}
	}

	string DtoB(int k, int s)                   // Returns the Binary form of the minterm/don'tcare
	{
		string bin;                            // the binary form
		int b = 0;                             // temporary value
		int n = k;                             // temporary value
		while (n)                             // Conversion is performed and stored in bin reversed using the well known method
		{
			b = n % 2;
			n = n / 2;
			if (b == 0)
				bin.push_back('0');
			else
				bin.push_back('1');
		}

		bin = fillinzeros(bin);                 // calls a function that fills remaining part of the binary form with zeros to insure that the size of the binary form is equal to the number of variables
		reverse(bin.begin(), bin.end());        // Reverse the reversed binary number.
		return bin;
	}
	string fillinzeros(string s)    //fills the remaining part of the binary form with zeros to insure that the size of the binary form is equal to the number of variables 
	{
		while (s.size() != n)
		{
			s.push_back('0');
		}
		return s;
	}

	void countones()           //Countes the 1s in each minterm
	{
		for (int i = 0; i < F.size(); i++)
			F[i].ones = count(F[i].binary.begin(), F[i].binary.end(), 1);
	}


	void compare() // Starts the comparison between the minterm throught the method explained in each step
	{
		forms g;            // temporary storage for the values inserted into the first col
		for (int u = 0;u < F.size();u++)    // stors the binary value of the minterms along with the number of ones into col 1 (or as called colb)
		{
			g.form = F[u].binary;
			g.ones = F[u].ones;
			colb.push_back(g);
		}
		for (int i = 0; i < n; i++)   //starts the loop till the last possible col which is n (The last possible col is n+1 but in this case the function will be 1 which is checked in the main validation)
		{
			comparison(colb);        // compares the minterms in each col and extract the merged minterms into the next col (or as called colb)
			getprime();              // extracts the unticked minterms as they are the prime implicants
			if (cola.empty())        // checks if the col (the one the comparison is performed on) is empty because in this case all prime implicants are extracted and there is no use carry on with the loop so it breaks. 
				break;
			else
			{
				colb = cola;       // Puts the new merged minterms into the col of comparison. erasing the old col.
				cola.clear();      // erase the following col.
			}                      // this way we ensure that we only have 2 cols at a time (saving memory) and we only perform comparison on one of them.
		}
	}
	void comparison(vector<forms> k)                //k is the col being under comparison
	{
		forms tmp;                                  // temporary storage
		for (int r = 0; r < k.size(); r++)               // the purpose of the 2 loops is to iterate over the whole col for each minterm in the col
			for (int j = r + 1; j < k.size(); j++)
			{
				if (k[j].ones - k[r].ones > 1)           // if the difference of ones becomes greater than 1 it breaks from the loop because longer iterations are useless
					break;
				if (differences(k[r].form, k[j].form))      // compares two minterms and determines whether or not they have only one difference
				{
					tmp.form = showDiff(k[r].form, k[j].form);   // merge the two minterms int tmp
					tmp.ones = k[r].ones;                        // stores the number of 1s in tmp
					colb[r].ticked = true;                       // ticks the original minterm in the col
					colb[j].ticked = true;                       // ticks the other original minterm in the col
					tmp.ticked = false;                          // sets the value for the new merged minterm as unticked
					cola.push_back(tmp);                         // stores tmp along with all its data into the new col
				}
			}
	}


	bool differences(string a, string b)    // compares two minterms and determines whether or not they have only one difference
	{
		int c = 0;                // intialize the number of differences to be 0
		for (int l = 0; l < a.size(); l++)     // loops over the 2 strings
		{
			if (a.at(l) != b.at(l))    // check the differences between them and increment the counter by 1 for each difference
				c++;
		}
		if (c == 1)                // if there is only one difference return true (they can merge)
			return true;
		else                      // if not then return false (they can't merge)
			return false;
	}

	string showDiff(string a, string b)       // takes 2 strings that has only one difference between them and change it to be '-'
	{
		for (int p = 0; p < a.size(); p++) // loops over the two strings 
			if (a.at(p) != b.at(p))        // change the difference between them to be '-', then breaks.
			{
				a.at(p) = '-';
				break;
			}
		return a;            //return the new merged minterm
	}

	void getprime()            // extract the unticked minterms from the first col (colb) after comparison as they are prime implicants
	{
		prime tmp;            // temporary storage;
		for (int i = 0; i < colb.size(); i++)  //loops over the whole col
		{
			if (colb[i].ticked == false)     // if the minterm is unticked it's stored in a new vector called primes
			{
				tmp.form = colb[i].form;
				primes.push_back(tmp);
			}
		}
	}

	void deleteprime()          //loops over the extracted primes and delete any repeated ones
	{
		for (int i = 0; i < primes.size(); i++)
			for (int j = i + 1; j < primes.size();j++)
				if (primes[i].form == primes[j].form)
				{
					primes.erase(primes.begin() + j);
					j--;                                      // because the size will decrease by one this means that when we increment we would have skipped 2 minterms. so when we decrease the size by one, we also decrease the counter
				}
	}


	void ConvertToVariables()                                // converts all the prime implicants into the form of variables. A, B, C, ...
	{
		char b;
		for (int j = 0; j < primes.size(); j++)               // loops over all the prime implicants
			for (int i = 0; i < n; i++)                       // loops over each form
			{
				if (primes[j].form.at(i) == '1')              // if 1 inserts the variable with reference to its ASCII code
				{
					b = 65 + i;
					primes[j].min.push_back(b);
				}
				else
					if (primes[j].form.at(i) == '0')        // if 0 appends the variable with '.
					{
						b = 65 + i;
						primes[j].min.push_back(b);
						primes[j].min.append("'");
					}                                          // the reason for the two conditions is to neglect the case when the char is '-'
			}
	}

	void diplayfunction()                                     // displays the prime and essential implicants
	{
		cout << "The function is a function of the variables: ";
		for (int i = 0; i < n; i++)
		{
			cout << char(65 + i) << " ";
		}
		cout << endl;
		cout << "The primes implicants are: " << endl;
		for (int i = 0; i < primes.size(); i++)                                  // display all the prime implicants
		{
			if (primes[i].care)
				cout << primes[i].form << "    " << primes[i].min << endl;
		}
		cout << "The essential prime implicants are:" << endl;
		for (int r = 0; r < primes.size(); r++)                                   // display the marked prime implicants (the essential ones)
			if (primes[r].mark)
				cout << primes[r].form << "   " << primes[r].min << endl;
	}


	bool checkexist(int index, string b)                  // it checks if a certian input minterm is included in (a part of) the prime implicant.
	{
		bool flag = false;
		int c = 0;                                       //sets the counter to 0;
		for (int l = 0; l < n; l++)                      // loops over the given string and the binary form of the index given
		{
			if (F[index].binary.at(l) != b.at(l))         // if there is a differnce check:
				if ((F[index].binary.at(l) != '-') && (b.at(l) != '-'))   // if this difference isn't '-'
					c++;                                                   // increment the counter
		}
		if (c == 0)                                // if there are no differences other than '-' return true
			flag = true;
		else                                       // if there are any differences other than '-' return false
			flag = false;
		return flag;
	}                                              // this simply saves the storage of extra information through out the tabulation process and at the end check the number of minterms included in each prime

	void getessential()                            // extracts the essential primes from the other primes
	{
		for (int i = 0; i < F.size(); i++)          // loops over the minterms
		{
			if (!F[i].dontcare)                     // checks only the not-dontcare-minterms
				for (int r = 0; r < primes.size(); r++)   // loops over the prime implicants
					if (checkexist(i, primes[r].form))    // for each minterm checks if it exists in how many primes
					{
						F[i].include++;
						primes[r].care = true;            // if a prime includes a minterm then it's a care prime
					}                                     // The last step is performed to avoid printing any primes that includes only don't cares
		}
		for (int i = 0; i < F.size(); i++)             // loops over all the minterms
			if (F[i].include == 1)                    // if it's only included in one prime
				for (int r = 0; r < primes.size(); r++)   // Then loop over the primes to find that prime
					if (checkexist(i, primes[r].form))
					{
						primes[r].mark = true;              // mark that prime to be essential
						primes[r].includedf = true;          // mark it to be included in the optimized function
					}
	}

	void clearcover()          // this function removes the used minterms and primes from the coveragetable
	{
		for (int i = 0; i < primes.size(); i++)   //loops over the primes
			if (primes[i].includedf)              // if the prime is included
			{
				for (int r = 0; r < F.size(); r++)        // loop over the minterms to find the coressponding minterms in the coverage table
					if (checkexist(r, primes[i].form))
						F[r].includef = true;               // assign them as included in the function
			}
	}


	void cover()    // this function creates the new coverage table after the used primes and minterms
	{
		for (int r = 0; r < primes.size(); r++)           //loops over the primes
			primes[r].included = 0;                       // set the number of included minterms from the coverage table to zero
		for (int i = 0; i < F.size(); i++)               // loops over the minterms
			if(!F[i].dontcare)                           // if it's not a don't care
			if (!F[i].includef)                          // if it's not yet included in the optimized function
				for (int r = 0; r < primes.size(); r++)  // loop over the primes
					if (checkexist(i, primes[r].form))  // check the corresponding primes
					{
						primes[r].included++;           // increase the value of the counter that counts the corresponding minterms in the coverage table.
					}
		sort(primes.begin(), primes.end());             // sort the primes based on the number of minterms included in them in the current coverage table
		for (int r = primes.size() - 1; r >= 0; r--)    // loops over the primes starting from the greatest one
			if (primes[r].care)                         // if it's a care prime (includes any minterm that is not a don't care)
			{
				primes[r].includedf = true;            // mark it as included in the optimized function
				break;                                // break from the loop
			}
	}

	void createcoverage()           //this function creates the coverage table and find the optimized function
	{
		int c = 0;                 // set the counter to 0
		bool flag = true;
		while (flag)
		{
			for (int i = 0; i < F.size();i++)      // loops over the minterms
				if (!F[i].dontcare)                // if it's not a don'tcare
					if (!F[i].includef)            // and it's not included yet in the main function
						c++;                      // increment the counter by 1
			if (c == 0)                          // if the counter is 0 (all the minterms are included in the optimized function)
				flag = false;                    // exit the coverage table
			clearcover();                       // calls the function that clears the coverage table
			cover();                            // calls the function that reconstructs the new coverage table
			c = 0;                              // reset the counter to 0
		}
	}

	void printfunction()         //print the optimized function 
	{
		int c = 0;                 // sets a temporary storage to 0
		cout << "F = ";
		for (int i = 0; i < primes.size(); i++)   // loops over the primes
		{
			if (primes[i].includedf == true)       // finds the first included prime
			{
				cout << primes[i].min;             //prints it
				c = i + 1;                         // store the value of the loop count +1
				break;                             // breaks from the loop
			}
		}
		for (int i = c; i < primes.size() ; i++)  // loops over the remaining primes
			if (primes[i].includedf == true)      // prints + followed by the included prime
				cout << " + " << primes[i].min;
		cout << endl;

	}


	void header()
	{
		for (int i = 0; i < 100; i++)
			cout << "*";
			cout << endl;
	}