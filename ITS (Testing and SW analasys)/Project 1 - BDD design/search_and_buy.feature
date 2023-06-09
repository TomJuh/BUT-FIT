Feature:
    Scenario: Searching products
        Given The store page is opened
        When  The user enters name of product in search bar 
        And   presses enter or clicks search
        Then  It displays products compatibile with the search quarie

    Scenario: Looking at quantity of product
        Given The product page is opened
        When  User chooses the amount of product to buy
        And   Clicks add to cart
        Then  Notification is shown
        And   Selected amount of product has been added to cart

    Scenario: Adding to cart without specification
        Given The product page is opened
        When  User clicks on add to cart button
        And   They are no <required> field to select from
        Then  Notifications is shown
        And   The product is added to cart

    Scenario: Adding to cart without specification
        Given The product page is opened
        And   It contains <required> fields
        When  User selects the required fields options
        And   Clicks add to cart
        Then  Notifications is shown
        And   The product is added to cart
        Examples:
            |required|
            |Radio:|
            |small  (+14$)|
            |medium (+26$)|
            |large  (+36)|

    Scenario: Selecting product
        Given Page with a product thumbnail is opened
        When  User Clicks on product thumbnail
        Then  Page of product is displayed
    
    Scenario: Buying product 
        Given Chekcout page is opened
        And   all <required> fields have been filled in
        When  User clicks on continue, fills in <required> delivery option
        And   clicks Confirm order
        Then  Notification of confirmation is displayed
        And   the order is created
        Examples:
            |required fields|
            |First name|
            |Last name|
            |Email|
        Examples:
            |required options|
            |Delivery method|
            |Payment method|
