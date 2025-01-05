/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 17:12:41 by dbogovic          #+#    #+#             */
/*   Updated: 2025/01/05 19:34:38 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../../minishell.h"


/*
remind Markus to put int last_Exe_Code to the struct!
*/



t_err execute(t_cmd_table *table)
{
	if (pipe_setup(table) == FAIL)	//after this point, close(fd) is also needed!!!
	{
		printf("fatal error! pipe setup failed!\n");
		return ERROR;
	}
	if (heredoc(table) == FAIL)//after this need to delete heredoc also
	{
		//closepipes();
		return ERROR;
	}

	//merge_heredoc_cmd;


return (OK);



}
