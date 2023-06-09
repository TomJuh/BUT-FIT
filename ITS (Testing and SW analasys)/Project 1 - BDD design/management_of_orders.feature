Feature:
    Scenario: Check orders
    Background: Admin is logged in administration page
        Given Admin has dashboard page opened
        When Sales menu button is clicked
        And  orders category selected
        Then A page displaying current orders is shown
    
    Scenario: Remove orders
    Background: Admin is logged in administration page
        Given Admin has current orders page opened
        And   has selected a order by clicking the checkbox button
        When  The delete button (trashcan) is clicked
        And   confirmation of action is clicked
        Then  A notification confirming deletion is shown
        And   the order is deleted from the system

    Scenario:   Adding orders
    Background: Admin is logged in administration page
        Given Admin has add order page opened
        And   has filled all required fields
        When  The confirm button is pressed 
        Then  A notification confirming addition is shown
        And   the order is added 
    
    Scenario:   Checking details of order
    Background: Admin is logged in administration page
        Given Admin has current orders page opened
        When  The view button (eye) is pressed
        Then  A page with order details
        And   edits is shown

    Scenario:   Adding  product to an order
    Background: Admin is logged in administration page
        Given Admin has edit order page opened
        And   has opened add item menu
        When  The new product is selected 
        And   the save button is pressed
        Then  A notification confirming addition is shown
        And   the product is added

    Scenario:   Deleting  product from an order
    Background: Admin is logged in administration page
        Given Admin has edit order page opened
        When  The delete product button is clicked
        Then  A notification confirming deletion is shown
        And   the product is deleted

    Scenario:  Edit shipping address
    Background: Admin is logged in administration page
        Given Admin has edit order page opened
        And   has selected edit shipping address
        When  Changes to any field are made
        And   the save button is pressed
        Then  A confirmation notification is shown
        And   the shipping address is changed