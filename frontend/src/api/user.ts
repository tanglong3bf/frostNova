import request from "@/utils/request";
import type {LoginForm, LoginResponse} from "@/api/auth";

export interface UserQuery {
    username?: string
    phoneNumber?: string
    status?: number
    dateRange: string[]
}

export interface pageQuery {
    page: number
    pageSize: number
}

export interface User {
    userId: number
    username: string
    nickname: string
    dept: {
        deptName: string
    }
    phoneNumber: string
    status: number
    createTime: string
    selected: boolean
}

export const getUserList = (query: UserQuery | pageQuery) => {
    console.log(query)
    return request.get<UserList, UserList, UserQuery | pageQuery>(
        '/user',
        {
            params: query
        })
}
