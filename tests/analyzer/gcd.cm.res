program
    func @ line 5
        type [int] @ line 5
        new_id [global_gcd] @ line 5
        parameters
            var_param @ line 5
                type [int] @ line 5
                new_id [global_gcd_u] @ line 5
            var_param @ line 5
                type [int] @ line 5
                new_id [global_gcd_v] @ line 5
        block
            declarations
            statements
                if_else @ line 6
                    == @ line 6
                        var [global_gcd_v] @ line 6
                        int [0] @ line 6
                    return @ line 6
                        var [global_gcd_u] @ line 6
                    return @ line 7
                        funccall @ line 7
                            var [global_gcd] @ line 7
                            arguments
                                var [global_gcd_v] @ line 7
                                - @ line 7
                                    var [global_gcd_u] @ line 7
                                    * @ line 7
                                        / @ line 7
                                            var [global_gcd_u] @ line 7
                                            var [global_gcd_v] @ line 7
                                        var [global_gcd_v] @ line 7
    func @ line 11
        type [void] @ line 11
        new_id [global_main] @ line 11
        parameters
        block
            declarations
                var_decl @ line 12
                    type [int] @ line 12
                    new_id [global_main_x] @ line 12
                var_decl @ line 12
                    type [int] @ line 12
                    new_id [global_main_y] @ line 12
            statements
                exprstmt @ line 13
                    = @ line 13
                        var [global_main_x] @ line 13
                        int [1] @ line 13
                exprstmt @ line 13
                    = @ line 13
                        var [y] @ line 13
                        int [2] @ line 13
                exprstmt @ line 14
                    funccall @ line 14
                        var [gcd] @ line 14
                        arguments
                            var [x] @ line 14
                            var [y] @ line 14
