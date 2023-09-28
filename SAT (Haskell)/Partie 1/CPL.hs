-- |
-- -- @Module : CPL
-- -- @Brief : Module avec les fonctions et type nécéssaire pour créer des formules et des mondes et les manipuler les ensembles.
-- -- @Date : 2023
-- -- @Description : Comporte toutes les fonctions et type / structures pour les formules , les mondes et les manipulations de ces types
module CPL
  ( Formula (..),
    World,
    clearDuplicate,
    genAllWorldsBis,
    genAllWorlds,
    sat,
    findWorlds,
    extract,
    test,
    testAll,
    testGenAllWorlds,
    testSat,
    testFindWorlds,
  )
where

-----------------------------------------------------------------------
--                           Les Types                               --
-----------------------------------------------------------------------

-- |
-- -- @Name : Formula
-- -- @Brief : Contient des prototype de formules pour créer des formules logiques
data Formula
  = T
  | F
  | Or Formula Formula
  | And Formula Formula
  | Not Formula
  | Eqv Formula Formula
  | Imp Formula Formula
  | Var String
  deriving (Eq, Show)

-- |
-- -- @Name : World
-- -- @Brief : Est un tableu de String
type World = [String]

-----------------------------------------------------------------------
--                           Les Fonctions                           --
-----------------------------------------------------------------------

-- |
-- -- @Name : clearDuplicate
-- -- @Brief : Retire les éléments présents plusieurs fois dans la liste afin de n'en laisser qu'un exemplaire
-- -- @Arguments : Une liste d'éléments
-- -- @Return : Une liste d'éléments
clearDuplicate :: Eq a => [a] -> [a]
clearDuplicate [] = []
clearDuplicate (x : xs)
  | x `elem` xs = clearDuplicate xs
  | otherwise = x : clearDuplicate xs

-- |
-- -- @Name : takeAwayElementFromList
-- -- @Brief : Prend l'element à l'indice du premier entier, le second est utilisé comme indice de l'élément courrant
-- -- @Arguments : Liste d'éléments, indice de l'élément à enlever, indice courant
-- -- @Return : La liste sans l'élément à retirer
takeAwayElementFromList :: [a] -> Int -> Int -> [a]
takeAwayElementFromList [] _ _ = []
takeAwayElementFromList (x : xs) k i
  | i == k = takeAwayElementFromList xs k (i + 1)
  | otherwise = x : takeAwayElementFromList xs k (i + 1)

-- |
-- -- @Name : genAllWorldsBis
-- -- @Brief : Fonction servant genAllWorlds, elle va générer toutes les partitions de World possiblement de façon dupliquées
-- -- @Arguments : Un World, et un indice de la variable courante
-- -- @Return : Les partitions de Worlds possiblement de façon dupliquées
genAllWorldsBis :: World -> Int -> [World]
genAllWorldsBis l i
  | i == length l = []
  | i == 0 = [l] ++ genAllWorldsBis (takeAwayElementFromList l i 0) 0 ++ genAllWorldsBis l (i + 1)
  | otherwise = genAllWorldsBis (takeAwayElementFromList l i 0) 0 ++ genAllWorldsBis l (i + 1)

-- |
-- -- @Name : genAllWorlds
-- -- @Brief : Permet de générer toutes les partitions d'un ensemble.
-- -- @Arguments : Un tableau de String (World)
-- -- @Return : Un tableau de tableau de String ([World])
genAllWorlds :: World -> [World]
genAllWorlds w = [] : clearDuplicate (genAllWorldsBis w 0)

-- |
-- -- @Name : sat
-- -- @Brief : Regarde si la formule est vrai ou non
-- -- @Arguments : Le monde (World) ainsi que le Monde (World)
-- -- @Return : Retourne True ou False
sat :: World -> Formula -> Bool
sat _ T = True
sat _ F = False
sat w (Var v)
  | v `elem` w = True
  | otherwise = False
sat w (And phi psi) = sat w phi && sat w psi
sat w (Or phi psi) = sat w phi || sat w psi
sat w (Not f) = not (sat w f)
sat w (Eqv phi psi) = sat w phi == sat w psi
sat w (Imp phi psi) = not (sat w phi) || sat w psi

-- |
-- -- @Name : extractBis
-- -- @Brief : Fonction servant à extract, elle donne les variables présentes dans la formule mais possiblement dupliquées
-- -- @Arguments : Une formule logique (Formula)
-- -- @Return : Les variables présentes dans la formule
extractBis :: Formula -> World
extractBis F = []
extractBis T = []
extractBis (Var p) = [p]
extractBis (And phi psi) = extractBis phi ++ extractBis psi
extractBis (Or phi psi) = extractBis phi ++ extractBis psi
extractBis (Eqv phi psi) = extractBis phi ++ extractBis psi
extractBis (Imp phi psi) = extractBis phi ++ extractBis psi
extractBis (Not f) = extractBis f

-- |
-- -- @Name : extract
-- -- @Brief : Fonction permettant d'extraire les variable présentes dans la formule
-- -- @Arguments : Une formule logique (Formula)
-- -- @Return : Les variables présentes dans la formule
extract :: Formula -> World
extract f = clearDuplicate (extractBis f)

-- |
-- -- @Name : findWorldsBis
-- -- @Brief : Fonction servant à findWorlds, elle fait la même chose
-- -- @Arguments : Une formule logique (Formula), un tableau de World
-- -- @Return : Les mondes valides ([World])
findWorldsBis :: Formula -> [World] -> [World]
findWorldsBis _ [] = []
findWorldsBis f (x : xs)
  | sat x f = x : findWorldsBis f xs
  | otherwise = findWorldsBis f xs

-- |
-- -- @Name : findWorlds
-- -- @Brief : Permet de générer les Worlds valides pour une formule
-- -- @Arguments : Une formule logique (Formula)
-- -- @Return : Les mondes valides ([World])
findWorlds :: Formula -> [World]
findWorlds f = findWorldsBis f (genAllWorlds (extract f))

-----------------------------------------------------------------------
--                           Les Tests                               --
-----------------------------------------------------------------------

-- |
-- -- @Name : testGenAllWorlds
-- -- @Brief : Permet de tester genAllWorlds
-- -- @Arguments : Un tableau de test ([Bool])
-- -- @Return : Rien
testGenAllWorlds :: [Bool]
testGenAllWorlds =
  [ genAllWorlds ["p1", "p2", "p3"] == [[], ["p1", "p2", "p3"], ["p2", "p3"], ["p1", "p3"], ["p3"], ["p1", "p2"], ["p2"], ["p1"]],
    genAllWorlds ["p1", "p2"] == [[], ["p1", "p2"], ["p2"], ["p1"]]
  ]

-- |
-- -- @Name : testFindWorlds
-- -- @Brief : Permet de tester findWorlds
-- -- @Arguments : Un tableau de test ([Bool])
-- -- @Return : Rien
testFindWorlds :: [Bool]
testFindWorlds =
  [ findWorlds (Not (Or (And (Var "t1") (Var "p1")) (And (Var "p1") (Var "t1")))) == [[], ["t1"], ["p1"]],
    null (findWorlds (And T F))
  ]

-- |
-- -- @Name : testGenAllWorlds
-- -- @Brief : Permet de tester genAllWorlds
-- -- @Arguments : Un tableau de test ([Bool])
-- -- @Return : Rien
testSat :: [Bool]
testSat =
  [ sat ["p1", "p2"] (And (Var "p1") (Var "p2")),
    not (sat ["p1", "p2"] (And (Var "p1") (Var "p3"))),
    sat ["p1", "p2"] (Or (Var "p1") (Var "p3")),
    not (sat ["p1", "p2"] (Or (Var "p3") (Var "p3"))),
    sat ["p1", "p2"] (Not (Var "p3")),
    not (sat ["p1", "p2"] (Not (Var "p1"))),
    sat ["p1", "p2"] (Eqv (Var "p1") (Var "p2")),
    not (sat ["p1", "p2"] (Eqv (Var "p1") (Var "p3"))),
    sat ["p1", "p2"] (Imp (Var "p1") (Var "p2")),
    not (sat ["p1", "p2"] (Imp (Var "p2") (Var "p3")))
  ]

-- |
-- -- @Name : test
-- -- @Brief : Permet de tester un ensemble de test
-- -- @Arguments : Un tableau de test ([Bool])
-- -- @Return : si les tests sont tous valides
test :: [Bool] -> Bool
test [] = True
test (l : ls)
  | null ls = True
  | not l = False
  | otherwise = test ls

-- |
-- -- @Name : testAll
-- -- @Brief : Permet de dire si les tests sont valides ou non
-- -- @Arguments : Rien
-- -- @Return : Le message du test
testAll :: [Char]
testAll
  | not
      (test testFindWorlds && test testSat) =
    "Failure"
  | otherwise = "Succes"