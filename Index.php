<?php 
    include_once 'include/head.inc.php';
    include_once 'include/function.inc.php';
    session_start();
?>

<script src="Ajax.js"> </script>
<?php
     printf('est définie: ' .isset($_SESSION['logins']) );
     printf("est vide: " .empty($_SESSION['logins']) );
    openBD();
    printf("login: " . $_SESSION['logins']);
    printf("role: " . $_SESSION['role']);

    printf("rep: "  . substr("test", 0,2));

    if(strcmp(substr("test", 0,2),"te") == 0)
    {
        printf("oui c bon");
    }
    include_once 'include/footeur.inc.php';
?>