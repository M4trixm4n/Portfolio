{-# OPTIONS_GHC -Wall #-}

--------------------------------------------------------------------------------

--------------------------------------------------------------------------------

-- |
-- Module     : TesterEp1
-- Copyright  : (c) Artois University 2023
-- Maintainer : tiago.delima@univ-artois.fr
--
-- Teste l'épisode 1 du projet de lambda-calcul et programation fonctionnelle.
module TesterEp1 where

import qualified CPL
import qualified Cha1 (challenge1)
import qualified Cha2 (challenge2)

--import qualified Cha3 (challenge3)
--import qualified Cha4 (challenge4)
--import qualified Cha5 (challenge5)
--import qualified Cha6 (challenge6)

-------------------------------------------------------------------------------

-- * Generic tests

subset :: Eq a => [a] -> [a] -> Bool
subset a b = and (map (\e -> e `elem` b) a)

eqList :: Eq a => [a] -> [a] -> Bool
eqList a b = (subset a b) && (subset b a)

subsetList :: Eq a => [[a]] -> [[a]] -> Bool
subsetList a b = and (map (\e -> e `elemList` b) a)

elemList :: Eq a => [a] -> [[a]] -> Bool
elemList e l = any (eqList e) l

eqListList :: Eq a => [[a]] -> [[a]] -> Bool
eqListList a b = (subsetList a b) && (subsetList b a)

-- | Tests a function with 1 argument.
testWith ::
  (Show a, Show b, Eq b) =>
  -- | Test name
  String ->
  -- | Function to compare results
  (b -> b -> Bool) ->
  -- | Function to be tested
  (a -> b) ->
  -- | List of pairs (argument, expected result)
  [(a, b)] ->
  -- | Returns errors or ""
  String
testWith name comp f ts =
  hline
    ++ "Testing: "
    ++ name
    ++ if (result == "")
      then ": OK\n"
      else
        (": ERRORS!!\n" ++ result)
          ++ hline
  where
    result = test' ts
    hline = (take 80 (repeat '-')) ++ "\n"
    test' [] = ""
    test' ((a, e) : ts') =
      let g = f a
       in ( if not (g `comp` e)
              then
                "\nArguments : " ++ show a ++ "\n"
                  ++ "Attendu   : "
                  ++ show e
                  ++ "\n"
                  ++ "Renvoyé   : "
                  ++ show g
                  ++ "\n"
              else ""
          )
            ++ test' ts'

-- | Tests a function with 1 argument.
test :: (Show a, Show b, Eq b) => String -> (a -> b) -> [(a, b)] -> String
test name f ts =
  hline
    ++ "Testing: "
    ++ name
    ++ "\n"
    ++ test' ts
    ++ hline
  where
    hline = (take 80 (repeat '-')) ++ "\n"
    test' [] = ""
    test' ((a, e) : ts') =
      let g = f a
       in ( if g /= e
              then
                "\nArguments : " ++ show a ++ "\n"
                  ++ "Attendu   : "
                  ++ show e
                  ++ "\n"
                  ++ "Renvoyé   : "
                  ++ show g
                  ++ "\n"
              else ""
          )
            ++ test' ts'

-- | Tests a function with 1 argument and returning a list.
testList :: (Show a, Show b, Ord b) => String -> (a -> [b]) -> [(a, [b])] -> String
testList name f ts =
  hline
    ++ "Testing: "
    ++ name
    ++ "\n"
    ++ test' ts
    ++ hline
  where
    hline = (take 80 (repeat '-')) ++ "\n"
    test' [] = ""
    test' ((a, e) : ts') =
      let g = f a
       in ( if not (eqList g e)
              then
                "\nArguments : " ++ show a ++ "\n"
                  ++ "Attendu   : "
                  ++ show e
                  ++ "\n"
                  ++ "Renvoyé   : "
                  ++ show g
                  ++ "\n"
              else ""
          )
            ++ test' ts'

-- | Tests a function with 1 argument and returning a list of list.
testListList :: (Show a, Show b, Ord b) => String -> (a -> [[b]]) -> [(a, [[b]])] -> String
testListList name f ts =
  hline
    ++ "Testing: "
    ++ name
    ++ "\n"
    ++ test' ts
    ++ hline
  where
    hline = (take 80 (repeat '-')) ++ "\n"
    test' [] = ""
    test' ((a, e) : ts') =
      let g = f a
       in ( if not (eqListList g e)
              then
                "\nArguments : " ++ show a ++ "\n"
                  ++ "Attendu   : "
                  ++ show e
                  ++ "\n"
                  ++ "Renvoyé   : "
                  ++ show g
                  ++ "\n"
              else ""
          )
            ++ test' ts'

-------------------------------------------------------------------------------

-- * Test lists

genAllWorlds_testList :: [([String], [CPL.World])]
genAllWorlds_testList =
  [ ([], [[]]),
    (["p0"], [[], ["p0"]]),
    (["p0", "p1"], [[], ["p1"], ["p0"], ["p0", "p1"]])
  ]

sat_testList :: [(CPL.Formula, Bool)]
sat_testList =
  [ (CPL.F, False),
    (CPL.Not (CPL.Var "p"), True),
    (CPL.Or (CPL.Var "p") (CPL.Not (CPL.Var "p")), True)
  ]

sat_testList2 :: [(CPL.Formula, Bool)]
sat_testList2 =
  [ (CPL.T, True),
    (CPL.Var "p", True),
    (CPL.Or (CPL.Var "p") (CPL.Not (CPL.Var "p")), True)
  ]

findWorlds_testList :: [(CPL.Formula, [CPL.World])]
findWorlds_testList =
  [ (CPL.T, [[]]),
    (CPL.Var "p", [["p"]]),
    ((CPL.Var "p") `CPL.And` (CPL.Var "q"), [["p", "q"]]),
    ((CPL.Not (CPL.Var "p")) `CPL.And` (CPL.Var "q"), [["q"]])
  ]

-------------------------------------------------------------------------------

-- * Challenges

--cha1,cha2,cha3,cha4,cha5,cha6 :: (CPL.Formula, [CPL.World])
cha1 :: (CPL.Formula, [CPL.World])
cha1 = (Cha1.challenge1, [["p2", "t1"]])

cha2 :: (CPL.Formula, [CPL.World])
cha2 = (Cha2.challenge2, [[], ["p1", "p2", "t1"], ["p2", "t1"], ["p1", "t1"]])

--------------------------------------------------------------------------------

-- * Run tests

-- | Runs all tests
main :: IO ()
main = do
  putStrLn "\n### Résultats de tests proposés par l'enseignant\n"
  putStrLn "```"
  putStrLn
    ( concat
        [ --testWith "CPL.genWorlds"    (eqListList) CPL.genWorlds       genAllWorlds_testList
          testWith "CPL.genAllWorlds" (eqListList) CPL.genAllWorlds genAllWorlds_testList,
          testWith "CPL.sat []" (==) (CPL.sat []) sat_testList,
          testWith "CPL.sat [p,q]" (==) (CPL.sat ["q", "p"]) sat_testList2,
          testWith "CPL.findWorlds" (eqListList) CPL.findWorlds findWorlds_testList,
          testWith "Challenge 1" (eqListList) CPL.findWorlds [cha1],
          testWith "Challenge 2" (eqListList) CPL.findWorlds [cha2]
          --, testWith "Challenge 3"      (eqListList) CPL.findWorlds    [cha3]
          --, testWith "Challenge 4"      (eqListList) CPL.findWorlds    [cha4]
          --, testWith "Challenge 5"      (eqListList) CPL.findWorlds    [cha5]
          --, testWith "Challenge 6"      (eqListList) CPL.findWorlds    [cha6]
        ]
    )
  putStrLn "```\n"
  putStrLn "### Résultats de tests proposés par les étudiants\n"
  putStrLn "```"
  putStrLn
    ( --   "genWorlds: " ++ show CPL.testGenWorlds ++ "\n"
      "genAllWorlds: " ++ show CPL.testGenAllWorlds ++ "\n"
        ++ "sat: "
        ++ show CPL.testSat
        ++ "\n"
        ++ "findWorlds: "
        ++ show CPL.testFindWorlds
        ++ "\n"
        ++ "all: "
        ++ show CPL.testAll
        ++ "\n"
        ++ "```\n"
    )
