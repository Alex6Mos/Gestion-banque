<?php
    include_once 'include/head.inc.php';
    include_once 'include/function.inc.php';
    session_start();
?>
    <article >
        <h2 id="h2Con"> Connexion </h2>
        <form method='post' id="conection">
            <div class='row'>
                <label for='login'> Login </label>
            </div>
            <div class='row'>
                <input type='text' name="login" id='login' required     />
            </div>
            <div class='row'>
                <label for='mdp' > Mot de passe </label>
            </div>
            <div class='row'>
                <input type='password' name='mdp' id='mdp' required     />
            </div>
            <div>
                <input type='submit'    class="btn btn-primary"     />
            </div>
        </form>
    </article>

    <script src="Ajax.js"> </script>
<?php
    if(isset($_POST['login']) && isset($_POST['mdp']))
    {
        connection();
    }
    include_once 'include/footeur.inc.php';
?>