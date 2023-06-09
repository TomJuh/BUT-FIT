Feature:
    Scenario:   Show list of customers
    Background: Admin is logged in administration page
        Given  Admin has dashboard page opened
        When   Customers menu is clicked
        And    Customers category selected
        Then   A page displaying all customers is shown

    Scenario:   Delete customer
    Background: Admin is logged in administration page
        Given  Admin has customers page opened
        And    has selected a checkbox button next to a customer
        When   The delete button is clicked 
        And    and confirmation is made
        Then   A notification is shown
        And    the customer is deleted from the system

    Scenario:   Adding customer
    Background: Admin is logged in administration page
        Given  Admin has add customers page opened
        And    has filled in all necessery fields 
        When   The save button is clicked 
        Then   A notification is shown
        And    the customer is added to the system

    Scenario:   Edit customer
    Background: Admin is logged in administration page
        Given  Admin has edit customer page opened
        And    all of the necessery fields are filled
        When   A modification is made
        And    the save button is pressed
        Then   A notification is shown
        And    the customer account information were modified

    Scenario:   Log into a user account
    Background: Admin is logged in administration page
        Given User has current customers page opened
        When  A dropdown menu under action is opened
        And   log into store option selected
        Then  A store page with customer account is loaded