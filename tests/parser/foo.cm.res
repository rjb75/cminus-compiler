program
    var_decl @ line 4
        type [int] @ line 4
        new_id [globala] @ line 4
    var_decl @ line 5
        type [int] @ line 5
        new_id [globalb] @ line 5
    array_decl @ line 6
        type [int] @ line 6
        new_id [globalarr] @ line 6
        int [10] @ line 6
    func @ line 10
        type [int] @ line 10
        new_id [divide] @ line 10
        parameters
            var_param @ line 10
                type [int] @ line 10
                new_id [a] @ line 10
            var_param @ line 10
                type [int] @ line 10
                new_id [b] @ line 10
        block
            declarations
            statements
                if @ line 12
                    == @ line 12
                        var [b] @ line 12
                        int [0] @ line 12
                    block
                        declarations
                        statements
                            return @ line 14
                                int [0] @ line 14
                return @ line 16
                    / @ line 16
                        var [a] @ line 16
                        var [b] @ line 16
    func @ line 20
        type [int] @ line 20
        new_id [multiply] @ line 20
        parameters
            var_param @ line 20
                type [int] @ line 20
                new_id [a] @ line 20
            var_param @ line 20
                type [int] @ line 20
                new_id [b] @ line 20
        block
            declarations
            statements
                return @ line 22
                    * @ line 22
                        var [a] @ line 22
                        var [b] @ line 22
    func @ line 26
        type [int] @ line 26
        new_id [add] @ line 26
        parameters
            var_param @ line 26
                type [int] @ line 26
                new_id [a] @ line 26
            var_param @ line 26
                type [int] @ line 26
                new_id [b] @ line 26
        block
            declarations
            statements
                return @ line 28
                    * @ line 28
                        var [a] @ line 28
                        var [b] @ line 28
    func @ line 32
        type [int] @ line 32
        new_id [subtract] @ line 32
        parameters
            var_param @ line 32
                type [int] @ line 32
                new_id [a] @ line 32
            var_param @ line 32
                type [int] @ line 32
                new_id [b] @ line 32
        block
            declarations
            statements
                return @ line 34
                    - @ line 34
                        var [a] @ line 34
                        var [b] @ line 34
    func @ line 37
        type [void] @ line 37
        new_id [main] @ line 37
        parameters
        block
            declarations
                var_decl @ line 39
                    type [int] @ line 39
                    new_id [high] @ line 39
                var_decl @ line 40
                    type [int] @ line 40
                    new_id [low] @ line 40
                var_decl @ line 41
                    type [int] @ line 41
                    new_id [i] @ line 41
                var_decl @ line 42
                    type [int] @ line 42
                    new_id [sum] @ line 42
            statements
                exprstmt @ line 44
                    = @ line 44
                        var [high] @ line 44
                        int [25] @ line 44
                exprstmt @ line 45
                    = @ line 45
                        var [low] @ line 45
                        int [5] @ line 45
                exprstmt @ line 47
                    = @ line 47
                        array_var [globalarr] @ line 47
                            int [0] @ line 47
                        funccall @ line 47
                            var [divide] @ line 47
                            arguments
                                var [high] @ line 47
                                var [low] @ line 47
                exprstmt @ line 48
                    = @ line 48
                        array_var [globalarr] @ line 48
                            int [1] @ line 48
                        funccall @ line 48
                            var [multiply] @ line 48
                            arguments
                                var [low] @ line 48
                                var [high] @ line 48
                exprstmt @ line 49
                    = @ line 49
                        array_var [globalarr] @ line 49
                            int [2] @ line 49
                        funccall @ line 49
                            var [add] @ line 49
                            arguments
                                var [high] @ line 49
                                var [low] @ line 49
                exprstmt @ line 50
                    = @ line 50
                        array_var [globalarr] @ line 50
                            int [3] @ line 50
                        funccall @ line 50
                            var [subtract] @ line 50
                            arguments
                                var [high] @ line 50
                                var [low] @ line 50
                exprstmt @ line 52
                    = @ line 52
                        var [i] @ line 52
                        int [4] @ line 52
                while @ line 53
                    <= @ line 53
                        var [i] @ line 53
                        int [9] @ line 53
                    block
                        declarations
                        statements
                            exprstmt @ line 55
                                = @ line 55
                                    array_var [globalarr] @ line 55
                                        var [i] @ line 55
                                    funccall @ line 55
                                        var [multiply] @ line 55
                                        arguments
                                            var [low] @ line 55
                                            var [i] @ line 55
                            exprstmt @ line 56
                                = @ line 56
                                    var [i] @ line 56
                                    + @ line 56
                                        var [i] @ line 56
                                        int [1] @ line 56
                exprstmt @ line 59
                    = @ line 59
                        var [i] @ line 59
                        int [0] @ line 59
                exprstmt @ line 60
                    = @ line 60
                        var [sum] @ line 60
                        int [0] @ line 60
                while @ line 61
                    < @ line 61
                        var [i] @ line 61
                        int [10] @ line 61
                    block
                        declarations
                        statements
                            exprstmt @ line 63
                                = @ line 63
                                    var [sum] @ line 63
                                    funccall @ line 63
                                        var [add] @ line 63
                                        arguments
                                            var [sum] @ line 63
                                            array_var [globalarr] @ line 63
                                                var [i] @ line 63
                return @ line 66
