<!DOCTYPE html>

<html lang='fr'>
<head>
    <title> <?php printf("" . $title); ?> </title>
    <meta charset='utf-8'   />
    <meta name='author' content="Etame Cedric &amp; Girard Vincent"    />
    <meta name='description' content="<?php printf("" . $descritpion);?>"   />

    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.4.1/css/bootstrap.min.css" integrity="sha384-Vkoo8x4CGsO3+Hhxv8T/Q5PaXtkKtu6ug5TOeNV6gBiFeWPGFN9MuhOf23Q9Ifjh" crossorigin="anonymous">
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.1.1/dist/js/bootstrap.min.js"></script> 
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.6.0/jquery.min.js"></script>
    <link rel="stylesheet" href="style.css"     />
</head>
<body id="test">
<header>
    <nav id="menu">
        <ul>
            <li> <a href="Index.php" > Accueil </a> </li>
            <li> <a href="Transaction.php" id="conecte" > Transaction </a> </li>
            <?php
                if(isset($_SESSION) != 1 && isset($_SESSION['logins']) != 1 && empty($_SESSION['logins']) == 1)
                {
                    printf("<li> <a href=\"conexion.php\" > Conexion </a> </li>");
                }

            ?>
        </ul>
    </nav>
</header>
