<?php
    
    function role(String $login):int
    {
        if(strcmp(substr($login, 0,2),"CO") == 0)
        {
            return 1;
        }
        else if(strcmp(substr($login, 0,2),"CL") == 0)
        {
            return 0;
        }
    return 3;
    }

    //Fonction d'ouverture de la BD
    function openBD()
    {
        //include a changer pour se connecter au différente BD
        include_once 'conf/BD_Vincent.conf.php';

        $pdo = new PDO("pgsql:host=". HOST .";port=5432;dbname=". BD,USER,MDP);

        //condition si se connecte pas a la BD
        if($pdo)
        {
        }
    return $pdo;
    }

    //Fonction qui verifie si le mot de passe et correcte selon le login
    function verifMdp($login, $mdp)
    {
        if(role($login) == 1)
        {
            $conn = openBD();

            //on cherche le mot de passe dans la BD avec le login rentrer par l'utilisateur
            $request = 'SELECT * FROM Conseiller WHERE id_conseiller = :logins';
            $prepare = $conn->prepare($request);
            $prepare->bindParam(":logins", $login);
            $prepare->execute();
            $res = $prepare->fetchAll(PDO::FETCH_OBJ);
            
            $mdpBD = $res[0]->mdp;

            //puis on verifie si les deux mdp corresponde
            if(password_verify($mdp, $mdpBD))
            {
                printf("mdp correcte");
                return true;
            }
        }
        else if(role($login) == 0)
        {
            $conn = openBD();

            $request = 'SELECT * FROM Clients WHERE id_client = :logins';
            $prepare = $conn->prepare($request);
            $prepare->bindParam(":logins", $login);
            $prepare->execute();
            $res = $prepare->fetchAll(PDO::FETCH_OBJ);
            
            $mdpBD = $res[0]->mdp;

            if(password_verify($mdp, $mdpBD))
            {
                printf("mdp correcte");
                return true;
            }
        }
    return false;
    }

    //permet de voir si le login existe bien dans la base de donnee
    function verifLogin(String $login)
    {
        // le cas d'un conseiller
        if(role($login) == 1)
        {
            $conn = openBD();

            $request = "SELECT * FROM Conseiller WHERE id_conseiller=:logins";
            $prepare = $conn->prepare($request);
            $prepare->bindParam(":logins", $login);
            $prepare->execute();

            $nbrRow = $prepare->rowCount();

            if($nbrRow == 1)
            {
                return true;
            }

        }
        //le cas d'un client
        else if(role($login) == 0)
        {
            $conn = openBD();

            $request = "SELECT * FROM Clients WHERE id_client=:logins";
            $prepare = $conn->prepare($request);
            $prepare->bindParam(":logins", $login);
            $prepare->execute();

            $nbrRow = $prepare->rowCount();

            if($nbrRow == 1)
            {
                return true;
            }
        }
    return false;
    }

    function rechercheRDVConseiller($login)
    {
        $conn = openBD();

        $request = "SELECT * FROM A_pris_rendez_vous_conseiller WHERE id_conseiller=:logins";
        $prepare = $conn->prepare($request);
        $prepare->bindParam(":login", $login);
        $prepare->execute();
        $res = $prepare->fetchAll();

        $nbrRow = $prepare->rowCount();
       

        for($i = 0; $i < $nbrRow; $i++ )
        {

        }
    }

    function rechercheRDVClient($login)
    {
        $conn = openBD();

        $request = "SELECT * FROM A_pris_rendez_vous_conseiller WHERE id_client=:logins";
        $prepare = $conn->prepare($request);
        $prepare->bindParam(":login", $login);
        $prepare->execute();
        $res = $prepare->fetchAll();
    }

    //fonction de conection du site
    function connection()
    {
        $mdp = $_POST['mdp'];
        $login = $_POST['login'];
        printf("Le login est: " .$login);

        if(verifLogin($login) && verifMdp($login,$mdp))
        {
            $_SESSION['logins'] = $login;
            $_SESSION['role'] = role($login);
            printf("conection réussi");
        }
        else
        {
            printf("conection non réussi");
        }
    }
    

?>