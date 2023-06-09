Feature:
    Scenario: Searching products
        Given The store page is opened
        When  The user enters name of product in search bar 
        And   presses enter or clicks search
        Then  It displays products compatibile with the search quarie

    Scenario: Adding to cart without specification
        Given The product page is opened
        When  User clicks add to cart
        Then  The product is added to cart

    Scenario: Buying product 
        Given Chekcout page is opened
        And   all <required> fields have been filled in
        When  User clicks on continue, fills in <required> delivery option
        And   clicks Confirm order
        Then  the order is created
