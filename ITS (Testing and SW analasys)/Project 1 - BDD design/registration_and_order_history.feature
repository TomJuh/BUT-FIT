Feature:
    Scenario: Registering with new order
    Background: User has selected a product
        Given User is on the checkout page
        When the register account option is selected
        And All required fields are filled
        And The order is confirmed
        Then A new user account is created with the order
        And the user has been logged in

    Scenario: Entering registration page
        Given User has store page opened
        When  User clicks on my account button
        And   Selects register
        Then  A registration page should be shown

    Scenario: Filling in registration
        Given User is on the new registration page
        And   all of the <required> field are filled in
        When  The privacy policy button has been checked
        And   User clicked on continue button
        Then  A new page confirming registration is displayed
        And   User has been loged in to his new account
        Examples:
            |required fields|
            |First name|
            |Last name|
            |Email|

    Scenario: Searching for order history
    Background: User has an account and is logged
        Given User is on the store page
        When User clicks on my account
        And Selects order history from the pop up menu
        Then Page with history of orders is displayed

    Scenario: Looking at details of order
    Background: User is registred and has ordered a product
        Given User is on the order history page
        When User clicks on the view button
        Then A page with all order details is displayed



