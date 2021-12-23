<?php
include_once 'include/head.inc.php';
session_start();
    session_destroy();
    //afficher les transaction d'un compte d'un client

    $mdp = password_hash('Vincent',PASSWORD_DEFAULT);
    printf("<p> le mdp est: " .$mdp . "</p>");
?>