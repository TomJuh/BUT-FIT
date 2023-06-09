Feature:

    Scenario: Check availible stock
    Background: Admin is logged in administration page
        Given Admin has dashboard opened
        When  Admin clicks on the catolog menu
        And   Selects products
        Then  A page with product description and quantity is shown
    Scenario: Adding a product 
    Background: Admin is logged in administration page
        Given Admin has add product page opened
        And   has filled in all required fields
        When  Clicks the save button 
        Then  A notification is shown to confirm addition
        And   a new product is displayed in the catolog

    Scenario: Deleting a product 
    Background: Admin is logged in administration page
        Given Admin has catalog of products page opened
        And   has selected a product by clicking a checkbox button
        When  And presses the delete button
        And   Confirms the action
        Then  A notification is shown to confirm addition
        And   the product is deleted from catalog 


    Scenario: Edit Product description
    Background: Admin is logged in administration page
        Given Admin has product edit page opened
        And   all neccesery fields are filled
        When  Admin rewrites a description
        And   clicks on Save icon 
        Then  A notification is shown to confirm change
        And   Product description is updated
 
    Scenario: Adding a category 
    Background: Admin is logged in administration page
        Given Admin has add category page opened
        And   has filled in all required fields
        When  Clicks the save button 
        Then  A notification is shown to confirm addition
        And   a new product is displayed in the catolog

    Scenario: Deleting a category 
    Background: Admin is logged in administration page
        Given Admin has catalog of categories page opened
        And   has selected a product by clicking a checkbox button
        When  And presses the delete button
        And   Confirms the action
        Then  A notification is shown to confirm addition
        And   the category is deleted from catalog 

    Scenario: Editing a categorie
    Background: Admin is logged in administration page
        Given Admin has edit page of category opened
        And   all the required fields are filled
        When  Admin clicks on the save icon
        Then  A confirmation notification is shown
        And   the catogory is rewritten
    